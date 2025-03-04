#include "bmpimage.hpp"

static bool operator==(const RGBTRIPLE& lhs, const RGBTRIPLE& rhs) {
    return lhs.rgbtBlue == rhs.rgbtBlue &&
           lhs.rgbtGreen == rhs.rgbtGreen &&
           lhs.rgbtRed == rhs.rgbtRed;
};

BMPImage::BMPImage(const std::string& filename) : bit_(0) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    HEADERS headers = {0};
    file.read(reinterpret_cast<char*>(&headers.fileHeader), sizeof(headers.fileHeader));
    file.read(reinterpret_cast<char*>(&headers.infoHeader), sizeof(headers.infoHeader));

    if (headers.fileHeader.bfType != bmpType) {
        throw std::runtime_error("Not a BMP file: " + filename);
    }
    bit_ = headers.infoHeader.biBitCount;
    std::cout<<"bit:"<<bit_<<"\n";
    if (bit_ != 24 && bit_ != 32) {
        throw std::runtime_error("Unsupported BMP bit depth: " + std::to_string(bit_));
    }
    width = headers.infoHeader.biWidth;
    height = headers.infoHeader.biHeight;

    pixels.resize(height, std::vector<RGBQUAD>(width));
    file.seekg(headers.fileHeader.bfOffBits, std::ios::beg);

    for (int y = height - 1; y >= 0; --y) {

        if (bit_ == 32){
            file.read(reinterpret_cast<char*>(pixels[y].data()), width * sizeof(RGBQUAD));    
        }else{
            for (int x = 0; x < width; ++x) {
                RGBTRIPLE triple;
                file.read(reinterpret_cast<char*>(&triple), sizeof(RGBTRIPLE));
                pixels[y][x] = {triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed, 0};
            }
            skipPadding(file);         
        }
    }
    file.close();
}

void BMPImage::display() const {
    for (const auto& row : pixels) {
        for (const auto& pixel : row) {
            RGBTRIPLE rgb = {pixel.rgbBlue, pixel.rgbGreen, pixel.rgbRed};
            std::cout << (rgb == RGBTRIPLE{0, 0, 0} ? "#" : " ");
        }
        std::cout << std::endl;
    }
}

void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
            pixels[y1][x1] = BLACK; 
        }

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void BMPImage::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not create file: " + filename);
    }

    HEADERS headers = createHeaders();

    file.write(reinterpret_cast<char*>(&headers.fileHeader), sizeof(headers.fileHeader));
    file.write(reinterpret_cast<char*>(&headers.infoHeader), sizeof(headers.infoHeader));

    for (int y = height - 1; y >= 0; --y) {
        if (bit_ == 32) {
            file.write(reinterpret_cast<const char*>(pixels[y].data()), width * sizeof(RGBQUAD));
        } else {
            for (int x = 0; x < width; ++x) {
                RGBTRIPLE triple = {pixels[y][x].rgbBlue, pixels[y][x].rgbGreen, pixels[y][x].rgbRed};
                file.write(reinterpret_cast<const char*>(&triple), sizeof(RGBTRIPLE));
            }
            addPadding(file);
        }
        }
    file.close();
}

void BMPImage::skipPadding(std::ifstream& file) const {
    int padding = calculateRowSize() - width * sizeof(RGBTRIPLE);
    if (padding > 0) {
        file.seekg(padding, std::ios::cur);
    }
}

void BMPImage::addPadding(std::ofstream& file) const {
    int padding = calculateRowSize() - width * sizeof(RGBTRIPLE);
    if (padding > 0) {
        const char zeroPadding[3] = {0};
        file.write(zeroPadding, padding);
    }
}

int BMPImage::calculateRowSize() const {
    if (bit_ == 32)
        return width * sizeof(RGBQUAD); 
    else
        return (width * sizeof(RGBTRIPLE) + 3) & (~3);
}

BMPImage::HEADERS BMPImage::createHeaders() const {
    HEADERS headers = {0};

    headers.fileHeader.bfType = bmpType;

    headers.fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + calculateRowSize() * height;
    
    headers.fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    headers.infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    headers.infoHeader.biWidth = width;
    headers.infoHeader.biHeight = height;
    headers.infoHeader.biPlanes = 1;
    headers.infoHeader.biBitCount = bit_;
    headers.infoHeader.biCompression = BI_RGB;

    return headers;
}
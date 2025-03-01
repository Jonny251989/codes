#include "bmpimage.h"

bool operator==(const RGBTRIPLE& lhs, const RGBTRIPLE& rhs) {
    return lhs.rgbtBlue == rhs.rgbtBlue &&
           lhs.rgbtGreen == rhs.rgbtGreen &&
           lhs.rgbtRed == rhs.rgbtRed;
}

BMPImage::BMPImage(const std::string& filename) {
    readBMP(filename);
}

void BMPImage::readBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.bfType != bmpType) {
        throw std::runtime_error("Not a BMP file: " + filename);
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;

    // Инициализация пикселей белым цветом
    pixels.resize(height);
    for (auto& row : pixels) {
        row.resize(width);
        std::fill(row.begin(), row.end(), WHITE);
    }

    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    int rowSize = (width * sizeof(RGBTRIPLE) + 3) & (~3); // Выравнивание по 4 байта

    // Чтение данных пикселей (снизу вверх)
    for (int y = height - 1; y >= 0; --y) {
        file.read(reinterpret_cast<char*>(pixels[y].data()), width * sizeof(RGBTRIPLE));
        file.seekg(rowSize - width * sizeof(RGBTRIPLE), std::ios::cur);
    }

    file.close();
}

// Запись BMP-файла
void BMPImage::writeBMP(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not create file: " + filename);
    }

    auto [fileHeader, infoHeader] = createHeaders();

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    int rowSize = (width * sizeof(RGBTRIPLE) + 3) & (~3);

    // Запись данных пикселей (снизу вверх)
    for (int y = height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<const char*>(pixels[y].data()), width * sizeof(RGBTRIPLE));
        if (rowSize > width * sizeof(RGBTRIPLE)) {
            char padding[3] = {0};
            file.write(padding, rowSize - width * sizeof(RGBTRIPLE));
        }
    }

    file.close();
}

// make header's BMP
std::pair<BITMAPFILEHEADER, BITMAPINFOHEADER> BMPImage::createHeaders() const {
    BITMAPFILEHEADER fileHeader = {0};
    BITMAPINFOHEADER infoHeader = {0};

    fileHeader.bfType = bmpType;
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * sizeof(RGBTRIPLE);
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24; // 24 бита на пиксель
    infoHeader.biCompression = BI_RGB; // Без сжатия

    return {fileHeader, infoHeader};
}

void BMPImage::display() const {
    for (const auto& row : pixels) {
        for (const auto& pixel : row) {
            std::cout << (pixel == BLACK ? "#" : " ");
        }
        std::cout << std::endl;
    }
}

// Bresenham's algorithm for drawing lines
void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        pixels[y1][x1] = BLACK;

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
    pixels[y1][x1] = BLACK; // last pixel
}

bool BMPImage::save(const std::string& filename) const {
    writeBMP(filename);
    return true;
}
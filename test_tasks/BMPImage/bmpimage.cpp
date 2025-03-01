#include "bmpimage.h"

BMPImage::BMPImage() : pixels(nullptr), width(0), height(0) {}

int BMPImage::getWidth() const { return width; }

int BMPImage::getHeight() const { return height; }

//Алгоритм Брезенхэма для отрисовки линий
void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        pixels[y1][x1] = {0, 0, 0}; // Черный цвет

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

bool BMPImage::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "Not a BMP file: " << filename << std::endl;
        return false;
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;

    pixels = new RGBTRIPLE*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new RGBTRIPLE[width];
    }

    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    int rowSize = (width * sizeof(RGBTRIPLE) + 3) & (~3); // Выравнивание по 4 байта

    for (int y = height - 1; y >= 0; --y) {
        file.read(reinterpret_cast<char*>(pixels[y]), width * sizeof(RGBTRIPLE));
        file.seekg(rowSize - width * sizeof(RGBTRIPLE), std::ios::cur); // Пропускаем выравнивание
    }

    file.close();
    return true;
}

void BMPImage::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (pixels[y][x].rgbtBlue == 0 && pixels[y][x].rgbtGreen == 0 && pixels[y][x].rgbtRed == 0) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool BMPImage::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not create file: " << filename << std::endl;
        return false;
    }

    BITMAPFILEHEADER fileHeader = {0};
    BITMAPINFOHEADER infoHeader = {0};

    fileHeader.bfType = 0x4D42;
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * sizeof(RGBTRIPLE); // Размер файла
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // Смещение до данных пикселей

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24; 
    infoHeader.biCompression = BI_RGB;

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    int rowSize = (width * sizeof(RGBTRIPLE) + 3) & (~3); 

    for (int y = height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<char*>(pixels[y]), width * sizeof(RGBTRIPLE));
        if (rowSize > width * sizeof(RGBTRIPLE)) {
            char padding[3] = {0};
            file.write(padding, rowSize - width * sizeof(RGBTRIPLE));
        }
    }
    file.close();
    return true;
}

BMPImage::~BMPImage() {
    if (pixels) {
        for (int i = 0; i < height; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
}
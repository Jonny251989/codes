#pragma once  

#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <windows.h>

class BMPImage {
public:

    BMPImage(const std::string& filename);

    void display() const;
    void drawLine(int x1, int y1, int x2, int y2);
    bool save(const std::string& filename) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    ~BMPImage() = default;

private:

    void readBMP(const std::string& filename);
    void writeBMP(const std::string& filename) const;
    std::pair<BITMAPFILEHEADER, BITMAPINFOHEADER> createHeaders() const;

    std::vector<std::vector<RGBTRIPLE>> pixels;
    static constexpr uint16_t bmpType = 0x4D42; 
    static constexpr RGBTRIPLE BLACK = {0, 0, 0};
    static constexpr RGBTRIPLE WHITE = {255, 255, 255}; 
    size_t width = 0;
    size_t height = 0;
};

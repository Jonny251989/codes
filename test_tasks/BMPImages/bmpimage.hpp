#pragma once  

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <windows.h>

class BMPImage {
    public:
        explicit BMPImage(const std::string& filename);
        BMPImage(const BMPImage& other) = default;
        BMPImage(BMPImage&& other) = default;

        void display() const;
        void drawLine(int x1, int y1, int x2, int y2);
        void save(const std::string& filename) const;
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        BMPImage& operator=(const BMPImage& other) = default;
        BMPImage& operator=(BMPImage&& other) = default;

        ~BMPImage() = default;
    private:
        struct HEADERS {
            BITMAPFILEHEADER fileHeader;
            BITMAPINFOHEADER infoHeader;
        };

        void skipPadding(std::ifstream& file) const;
        void addPadding(std::ofstream& file) const;
        int calculateRowSize() const;
        HEADERS createHeaders() const;

        static constexpr uint16_t bmpType = 0x4D42; 
        static constexpr RGBQUAD BLACK = {0, 0, 0, 0}; 
        size_t bit_;  
        size_t width = 0;
        size_t height = 0;
        std::vector<std::vector<RGBQUAD>> pixels;
    };
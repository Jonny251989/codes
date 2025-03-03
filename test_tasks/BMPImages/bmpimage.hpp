#pragma once  

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <windows.h>

inline bool operator==(const RGBTRIPLE& lhs, const RGBTRIPLE& rhs) {
    return lhs.rgbtBlue == rhs.rgbtBlue &&
           lhs.rgbtGreen == rhs.rgbtGreen &&
           lhs.rgbtRed == rhs.rgbtRed;
};

class BMPImage {
    public:
        BMPImage(const std::string& filename);
        void display() const;
        void drawLine(int x1, int y1, int x2, int y2);
        void save(const std::string& filename) const;
        int getWidth() const { return width; }
        int getHeight() const { return height; }
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
        std::vector<std::vector<RGBQUAD>> pixels;
        static constexpr uint16_t bmpType = 0x4D42; 
        static constexpr RGBQUAD BLACK = {0, 0, 0, 0}; 
        static constexpr RGBQUAD WHITE = {255, 255, 255, 0}; 
        size_t bit_;  
        size_t width = 0;
        size_t height = 0;
    };
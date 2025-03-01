#pragma once  

#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

class BMPImage {
    
    public:
        BMPImage();
        bool load(const std::string& filename);
        void display() const;
        void drawLine(int x1, int y1, int x2, int y2);
        bool save(const std::string& filename) const;
        int getWidth() const;
        int getHeight() const;
        ~BMPImage();
    
    private:
        RGBTRIPLE** pixels;
        int width, height;
    };
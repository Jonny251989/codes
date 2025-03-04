#include "bmpimage.hpp"
#include "windows.h"
#include <iostream>
#include <fstream>

int main() {

    try {
        std::string filename;
        std::cout << "Enter input BMP file name: ";
        std::cin >> filename;

        BMPImage image(filename);

        image.display();

        // Draw a cross (two diagonal lines)
        image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1); 
        image.drawLine(image.getWidth() - 1, 0, 0, image.getHeight() - 1); 

        image.display();

        std::cout << "Enter output BMP file name: ";
        std::cin >> filename;

        image.save(filename);
 
        std::cout << "Image saved successfully." << std::endl;
        
    }catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1; 
    }

    return 0; 
}
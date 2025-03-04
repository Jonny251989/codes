#include "bmpimage.hpp"
#include "windows.h"
#include <iostream>
#include <fstream>

int main() {
    std::ios::sync_with_stdio(false); // Disable synchronization with C-style I/O for better performance

    std::string filename;

    try {

        std::cout << "Enter input BMP file name: ";
        std::cin >> filename;

        BMPImage image(filename);

        image.display();

        // Draw a cross (two diagonal lines)
        image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1); // Line from top-left to bottom-right
        image.drawLine(image.getWidth() - 1, 0, 0, image.getHeight() - 1); // Line from top-right to bottom-left

        image.display();

        std::cout << "Enter output BMP file name: ";
        std::cin >> filename;

        image.save(filename);
 
        std::cout << "Image saved successfully." << std::endl;

    }catch (const std::runtime_error& e) {
        // Handle exceptions thrown by the BMPImage class (e.g., file errors)
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }catch (const std::exception& e) {
        // Handle any other unexpected exceptions
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1; 
    }

    return 0; 
}
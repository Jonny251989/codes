#include "bmpimage.h"
#include "windows.h"
#include <iostream>
#include <fstream>

int main() {

    std::ios::sync_with_stdio(false);

    BMPImage image;
    std::string filename;

    std::cout << "Enter input BMP file name: ";
    std::cin >> filename;

    if (!image.load(filename)) {
        return 1;
    }

    image.display();
    std::cout<<"\n";

    // Рисуем крест (две диагональные линии)
    image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1); // Линия из левого верхнего в правый нижний угол
    image.drawLine(image.getWidth() - 1, 0, 0, image.getHeight() - 1); // Линия из правого верхнего в левый нижний угол

    image.display();

    std::cout << "Enter output BMP file name: ";
    std::cin >> filename;

    if (!image.save(filename)) {
        return 1;
    }

    return 0;
}
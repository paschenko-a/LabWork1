#include "bmp_image.h"
#include <iostream>
#include <string>
#include <filesystem>

int main() {
    std::string path;
    std::cout << "Enter path to BMP file (press Enter for sample.bmp): ";
    std::getline(std::cin, path);

    if (path.empty()) {
        path = "sample.bmp";
    }

    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: File '" << path << "' not found!" << std::endl;
        return 1;
    }

    BMP img;
    if (!img.Load(path)) {
        std::cerr << "Error: Failed to load BMP!" << std::endl;
        return 1;
    }

    std::cout << "Processing " << path << "..." << std::endl;

    img.RotateClockwise().Save("clockwise.bmp");
    
    img.RotateCounterClockwise().Save("counter_clockwise.bmp");
    
    img.RotateClockwise().ApplyGaussianFilter().Save("gaussian.bmp");

    std::cout << "Done! Results saved to clockwise.bmp, counter_clockwise.bmp, and gaussian.bmp" << std::endl;

    return 0;
}
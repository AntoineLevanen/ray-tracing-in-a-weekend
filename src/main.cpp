#include <iostream>
#include <fstream>

#include "color.hpp"
#include "vector3.hpp"

int main() {

    // Image
    int image_width = 256;
    int image_height = 256;

    // Create a PPM image file
    std::ofstream ppmFile("../render/output.ppm");

    // Write the PPM header
    ppmFile << "P3" << std::endl;
    ppmFile << image_width << " " << image_height << std::endl;
    ppmFile << "255" << std::endl;

    for (int j = 0; j < image_height; j++) 
    {
        std::clog 
            << "\rScanlines remaining: " 
            << (image_height - j) 
            << ' ' << std::flush;
        
        for (int i = 0; i < image_width; i++) 
        {
            Color pixel_color = Color(
                double(i)/(image_width - 1), 
                double(j)/(image_height - 1), 
                0.0f);
 
            // Write RGB values to the PPM file
            write_color(ppmFile, pixel_color);
        }
        // End the line here
        ppmFile << std::endl;
    }

    // Close the PPM file
    ppmFile.close();

    std::clog << "\rDone.                 \n";

    return 0;
}
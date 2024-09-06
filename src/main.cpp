#include <iostream>
#include <fstream>

int main() {

    // Image

    int image_width = 256;
    int image_height = 256;
    // int image_width = 1600;
    // int image_height = 1600;

    // Create a PPM image file
    std::ofstream ppmFile("../render/output.ppm");


    // Render
    // To output the image to the console
    // std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Write the PPM header
    ppmFile << "P3" << std::endl;
    ppmFile << image_width << " " << image_height << std::endl;
    ppmFile << "255" << std::endl;

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            // To output the image to the console
            // std::cout << ir << ' ' << ig << ' ' << ib << '\n';
            // Write RGB values to the PPM file
            ppmFile << ir << ' ' << ig << ' ' << ib << " ";
        }
        // End the line here
        ppmFile << std::endl;
    }

    // Close the PPM file
    ppmFile.close();

    std::clog << "\rDone.                 \n";

    return 0;
}
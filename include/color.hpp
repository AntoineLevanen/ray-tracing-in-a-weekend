#pragma once

#include <iostream>
#include <fstream>

#include "vector3.hpp"

using Color = Vector3;

/**
 * write_color function to push the image to standard output, 
 * usefull if the output is redirected to a file.
 */
void write_color(std::ostream& out, const Color& pixel_color)
{
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Map the [0, 1] component values to byte range [0, 255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

/**
 * write_color function to write colors to an already existing image file.
 */
void write_color(std::ofstream& out, const Color& pixel_color)
{
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Map the [0, 1] component values to byte range [0, 255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
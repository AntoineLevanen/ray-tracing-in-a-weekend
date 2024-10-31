#pragma once

#include <fstream>

#include "interval.hpp"
#include "vector3.hpp"

using Color = Vector3;

inline double lineraToGamma(double linear_component)
{
  if (linear_component > 0)
  {
    return std::sqrt(linear_component);
  }
  return 0.0;
}

/**
 * write_color function to push the image to standard output, 
 * usefull if the output is redirected to a file.
 */
void write_color(std::ostream& out, const Color& pixel_color)
{
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();

  // Apply a linear to gamma transform for gamma 2
  r = lineraToGamma(r);
  g = lineraToGamma(g);
  b = lineraToGamma(b);

  // Map the [0, 1] component values to byte range [0, 255]
  static const Interval intensity(0.000, 0.999);
  int rbyte = int(256 * intensity.clamp(r));
  int gbyte = int(256 * intensity.clamp(g));
  int bbyte = int(256 * intensity.clamp(b));

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

  // Apply a linear to gamma transform for gamma 2
  r = lineraToGamma(r);
  g = lineraToGamma(g);
  b = lineraToGamma(b);

  // Map the [0, 1] component values to byte range [0, 255]
  static const Interval intensity(0.000, 0.999);
  int rbyte = int(256 * intensity.clamp(r));
  int gbyte = int(256 * intensity.clamp(g));
  int bbyte = int(256 * intensity.clamp(b));

  // Write out the pixel color components
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
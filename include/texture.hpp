#pragma once

#include "perlin.hpp"
#include "rtw_stb_image.h"

class Texture
{
  public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point3& point) const = 0;
};

class SolidColor : public Texture
{
  public:
    SolidColor(const Color& albedo) : albedo(albedo) {}

    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

    Color value(double u, double v, const Point3& point) const override
    {
      return albedo;
    }

  private:
    Color albedo;
};

class CheckerTexture : public Texture
{
  public:
    CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    CheckerTexture(double scale, const Color& c1, const Color& c2)
      : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}
    
    Color value(double u, double v, const Point3& point) const override
    {
      auto x_integer = int(std::floor(inv_scale * point.x()));
      auto y_integer = int(std::floor(inv_scale * point.y()));
      auto z_integer = int(std::floor(inv_scale * point.z()));

      bool is_even = (x_integer + y_integer + z_integer) % 2 == 0;

      return is_even ? even->value(u, v, point) : odd->value(u, v, point);
    }

  private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class ImageTexture : public Texture
{
  public:
    ImageTexture(const char* filename) : image(filename) {}

    Color value(double u, double v, const Point3& point) const override
    {
      // If we have no texture data, then return solid cyan as a debugging aid.
      if(image.height() <= 0) return Color(0, 1, 1);

      // Clamp input texture coordinates to [0, 1] x [1, 0]
      u = Interval(0, 1).clamp(u);
      v = 1.0 - Interval(0, 1).clamp(v); // Flip V to image coordinates

      auto i = int(u * image.width()-1);
      auto j = int(v * image.height()-1);
      auto pixel = image.pixelData(i, j);

      auto color_scale = 1.0 / 255.0;
      return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

  private:
    RTWImage image;
};

class NoiseTexture : public Texture
{
  public:
    NoiseTexture(double scale) : scale(scale) {}

    Color value(double u, double v, const Point3& point) const override
    {
      return Color(0.5, 0.5, 0.5) * (1 + std::sin(scale * point.z() + 10 * noise.turbulence(point, 7)));
    }
  
  private:
    Perlin noise;
    double scale;
};
#pragma once

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
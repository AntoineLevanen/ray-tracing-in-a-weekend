#pragma once

#include "hittable.hpp"
#include "rtweekend.hpp"

class Material
{
  public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const
    {
      return false;
    }
};

class Lambertian : public Material
{
private:
  Color albedo;

public:
  Lambertian(const Color& albedo) : albedo(albedo){};

  bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
  {
    Vector3 scatter_direction = record.normal + randomUnitVector();

    // Catch degenerate scatter direction
    if (scatter_direction.nearZero())
    {
      scatter_direction = record.normal;
    }

    scattered = Ray(record.hit_impact, scatter_direction);
    attenuation = albedo;
    return true;
  }
};

class Metal : public Material
{
  private:
    Color albedo;
    double fuzz;
  public:
    Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    
    bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
      Vector3 reflected = reflect(ray_in.direction(), record.normal);
      reflected = unit_vector(reflected) + (fuzz * randomUnitVector());
      scattered = Ray(record.hit_impact, reflected);
      attenuation = albedo;
      return (dot(scattered.direction(), record.normal) > 0);
    }
};

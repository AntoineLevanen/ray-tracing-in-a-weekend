#pragma once

#include "hittable.hpp"
#include "texture.hpp"
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
  shared_ptr<Texture> texture;

public:
  Lambertian(const Color& albedo) : texture(make_shared<SolidColor>(albedo)){};
  Lambertian(shared_ptr<Texture> texture) : texture(texture) {}

  bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
  {
    Vector3 scatter_direction = record.normal + randomUnitVector();

    // Catch degenerate scatter direction
    if (scatter_direction.nearZero())
    {
      scatter_direction = record.normal;
    }

    scattered = Ray(record.hit_impact, scatter_direction, ray_in.time());
    attenuation = texture->value(record.u, record.v, record.hit_impact);
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
      scattered = Ray(record.hit_impact, reflected, ray_in.time());
      attenuation = albedo;
      return (dot(scattered.direction(), record.normal) > 0);
    }
};

class Dielectric : public Material
{
  public:
    Dielectric(double refraction_index) : refraction_index(refraction_index){}

    bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override 
    {
      attenuation = Color(1.0, 1.0, 1.0);
      double ri = record.front_face ? (1.0 / refraction_index) : refraction_index;

      Vector3 unit_direction = unit_vector(ray_in.direction());
      double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
      double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

      bool cannot_refract = ri * sin_theta > 1.0;
      
      Vector3 direction;

      if(cannot_refract || reflectance(cos_theta, ri) > randomDouble())
      {
        direction = reflect(unit_direction, record.normal);
      }
      else
      {
        direction =  refract(unit_direction, record.normal, ri);
      }

      scattered = Ray(record.hit_impact, direction, ray_in.time());
      return true;
    }
  
  private:
    // The refractive index in vaccum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    static double reflectance(double cosine, double refraction_index)
    {
      // Use Schlick's approximation for reflectance.
      auto r0 = (1 - refraction_index) / (1 + refraction_index);
      r0 = r0 * r0;
      return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

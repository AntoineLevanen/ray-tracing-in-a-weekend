#pragma once

#include "hittable.hpp"

class Sphere : public Hittable
{
  public:
    Sphere(const Point3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override 
    {
      Vector3 oc = center - ray.origin();
      auto a = ray.direction().length_squared();
      auto h = dot(ray.direction(), oc);
      auto c = oc.length_squared() - radius*radius;

      auto discriminant = h*h - a*c;
      if (discriminant < 0)
      {
        return false;
      }

      auto sqrtd = std::sqrt(discriminant);

      // Find the nearest root that lies in the acceptable range.
      auto root = (h - sqrtd) / a;
      if (!ray_t.surrounds(root))
      {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
          return false;
        }
      }

      record.t = root;
      record.p = ray.at(record.t);
      Vector3 outward_normal = (record.p - center) / radius;
      record.setFaceNormal(ray, outward_normal);

      return true;
    }
  
  private:
    Point3 center;
    double radius;
};
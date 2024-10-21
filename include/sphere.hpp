#pragma once

#include "hittable.hpp"

class Sphere : public Hittable
{
  public:
    Sphere(const Point3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const Ray &r, double ray_tmin, double ray_tmax, HitRecord &record) const override {
      Vector3 oc = center - r.origin();
      auto a = r.direction().length_squared();
      auto h = dot(r.direction(), oc);
      auto c = oc.length_squared() - radius*radius;

      auto discriminant = h*h - a*c;
      if (discriminant < 0)
      {
        return false;
      }

      auto sqrtd = std::sqrt(discriminant);

      // Find the nearest root that lies in the acceptable range.
      auto root = (h - sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root){
        root = (h + sqrtd) / a;
        if (root <= ray_tmax <= root)
        {
          return false;
        }
      }

      record.t = root;
      record.p = r.at(record.t);
      Vector3 outward_normal = (record.p - center) / radius;
      record.setFaceNormal(r, outward_normal);

      return true;
    }
  
  private:
    Point3 center;
    double radius;
};
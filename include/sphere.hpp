#pragma once

#include "hittable.hpp"

class Sphere : public Hittable
{
  public:
    // Stationary Sphere
    Sphere(const Point3 &static_center, double radius, shared_ptr<Material> material) 
      : center(static_center, Vector3(0,0,0)), radius(std::fmax(0, radius)) , material(material) {}

    // Stationary Sphere
    Sphere(const Point3 &center1, const Point3 &center2, double radius, shared_ptr<Material> material) 
      : center(center1, center2 - center1), radius(std::fmax(0, radius)) , material(material) {}

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override 
    {
      Point3 current_center = center.at(ray.time());
      Vector3 oc = current_center - ray.origin();
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
      record.hit_impact= ray.at(record.t);
      Vector3 outward_normal = (record.hit_impact- current_center) / radius;
      record.setFaceNormal(ray, outward_normal);
      record.material = material;

      return true;
    }
  
  private:
    Ray center;
    double radius;
    shared_ptr<Material> material;
};
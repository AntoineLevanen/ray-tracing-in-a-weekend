#pragma once

#include "hittable.hpp"

class Sphere : public Hittable
{
  public:
    // Stationary Sphere
    Sphere(const Point3 &static_center, double radius, shared_ptr<Material> material) 
      : center(static_center, Vector3(0,0,0)), radius(std::fmax(0, radius)) , material(material) 
    {
      auto rvec = Vector3(radius, radius, radius);
      bbox = AABB(static_center - rvec, static_center + rvec);
    }

    // Stationary Sphere
    Sphere(const Point3 &center1, const Point3 &center2, double radius, shared_ptr<Material> material) 
      : center(center1, center2 - center1), radius(std::fmax(0, radius)) , material(material)
    {
      auto rvec = Vector3(radius, radius, radius);
      AABB box1(center.at(0) - rvec, center.at(0) + rvec);
      AABB box2(center.at(1) - rvec, center.at(1) + rvec);
      bbox = AABB(box1, box2);
    }

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
      getSphereUV(outward_normal, record.u, record.v);
      record.material = material;

      return true;
    }

    AABB boundingBox() const override
    {
      return bbox;
    }
  
  private:
    Ray center;
    double radius;
    shared_ptr<Material> material;
    AABB bbox;

    static void getSphereUV(const Point3& point, double& u, double& v)
    {
      // point : a given point on the sphere of radius one, centered at the origin.
      // u : returned value [0, 1] of an angle around the Y axis from X=-1
      // v : returned value [0, 1] of an angle from Y=-1 to X=-1
      // <1 0 0> yields <0.50 0.50>       <-1 0 0> yields <0.00 0.50>
      // <0 1 0> yields <0.50 1.00>       <0 -1 0> yields <0.50 0.00>
      // <0 0 1> yields <0.25 0.50>       <0 0 -1> yields <0.75 0.50>

      double theta = std::acos(-point.y());
      double phi = std::atan2(-point.z(), point.x()) + PI;

      u = phi / (2 * PI);
      v = theta / PI;
    }
};
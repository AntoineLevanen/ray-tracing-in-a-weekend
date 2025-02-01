#pragma once

#include "aabb.hpp"

class Material; // Define the Material class here to avoid circular reference issue.

class HitRecord
{
  public:
    Point3 hit_impact;
    Vector3 normal;
    shared_ptr<Material> material;
    double t;
    double u;
    double v;
    bool front_face;

    void setFaceNormal(const Ray &ray, const Vector3 &outward_normal)
    {
      // Set the hit record normal vector.
      // NOTE: the parameter 'outward_normal' is assumed to have unit length.

      front_face = dot(ray.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
  public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const = 0;

    virtual AABB boundingBox() const = 0;
};
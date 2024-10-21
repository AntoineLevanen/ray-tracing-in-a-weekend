#pragma once

class HitRecord
{
  public:
    Point3 p;
    Vector3 normal;
    double t;
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
    virtual bool hit(const Ray &r, double ray_tmin, double ray_tmax, HitRecord &record) const = 0;
};
#pragma once

#include "hittable.hpp"

class Quad : public Hittable
{
  public:
    Quad(const Point3& Q, const Vector3& u, const Vector3& v, shared_ptr<Material> material) : Q(Q), u(u), v(v), material(material)
    {
      auto n = cross(u, v);
      normal = unit_vector(n);
      D = dot(normal, Q);
      w = n / dot(n, n);

      setBoundingBox();
    }

    virtual void setBoundingBox()
    {
      // Compute the bounding box of all four vertices.

      auto bbox_diagonal1 = AABB(Q, Q + u + v);
      auto bbox_diagonal2 = AABB(Q + u, Q + v);
      bbox = AABB(bbox_diagonal1, bbox_diagonal2);
    }

    AABB boundingBox() const override { return bbox; }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override
    {
      double denominator = dot(normal, ray.direction());

      // No hit if the ray is parallel to the plane.
      if (std::fabs(denominator) < 1e-8)
        return false;
      
      // Return false if the hit point parameter t is outside the ray interval.
      auto t = (D - dot(normal, ray.origin())) / denominator;
      if(!ray_t.constains(t))
        return false;
      
      // Determine if the hit point lies within the planar shape using its plane coordiantes
      auto intersection = ray.at(t);
      Vector3 planar_hitpt_vector = intersection - Q;
      double alpha = dot(w, cross(planar_hitpt_vector, v));
      double beta = dot(w, cross(u, planar_hitpt_vector));

      if(!isInterior(alpha, beta, record))
        return false;

      record.t = t;
      record.hit_impact = intersection;
      record.material = material;
      record.setFaceNormal(ray, normal);

      return true;
    }

  virtual bool isInterior(double alpha, double beta, HitRecord& record) const
  {
    Interval unit_interval = Interval(0, 1);
    // Given the hit point in plane coordinates, return false if it is outside the 
    // primitive, otherwise set the hit record UV coordiantes and return true.

    if(!unit_interval.constains(alpha) || !unit_interval.constains(beta))
      return false;

    record.u = alpha;
    record.v = beta;
    return true;
  }
  
  private:
    Point3 Q;
    Vector3 u, v;
    Vector3 w;
    shared_ptr<Material> material;
    AABB bbox;
    Vector3 normal;
    double D;
};
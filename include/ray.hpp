#pragma once

#include "vector3.hpp"

class Ray
{
public:
  Ray() {}

  Ray(const Point3& origin, const Vector3& direction) 
    : ray_origin(origin), ray_direction(direction) {}

  Ray(const Point3& origin, const Vector3& direction, double time) 
    : ray_origin(origin), ray_direction(direction), ray_time(time) {}

  const Point3& origin() const { return ray_origin; }
  const Vector3& direction() const { return ray_direction; }
  double time() const { return ray_time; }

  Point3 at(double t) const
  {
    return ray_origin + t * ray_direction;
  }

private:
  Point3 ray_origin;
  Vector3 ray_direction;
  double ray_time;
};
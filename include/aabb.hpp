#pragma once

class AABB
{
  public:
    Interval x, y, z;

    AABB() {} // The default AABB is empty, since intervals are empty by default.

    AABB(const Interval& x, const Interval& y, const Interval& z)
      : x(x), y(y), z(z) {}

    AABB(const Point3& a, const Point3& b)
    {
      // Treat the two points a and b as extrema for the bounding box, so we don't
      // require a particular minimum or maximum coordinate order.

      x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]); 
      y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]); 
      z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }

    AABB(const AABB& box0, const AABB box1)
    {
      x = Interval(box0.x, box1.x);
      y = Interval(box0.y, box1.y);
      z = Interval(box0.z, box1.z);
    }

    const Interval& axisInterval(int n) const
    {
      if(n == 1) return y;
      if(n == 2) return z;
      return x;
    }

    bool hit(const Ray& ray, Interval ray_time) const
    {
      const Point3& ray_origin = ray.origin();
      const Vector3& ray_direction = ray.direction();

      for(int axis = 0; axis < 3; axis++)
      {
        const Interval& ax = axisInterval(axis);
        const double adinv = 1.0 / ray_direction[axis];

        double t0 = (ax.min - ray_origin[axis]) * adinv;
        double t1 = (ax.max - ray_origin[axis]) * adinv;

        if(t0 < t1)
        {
          if(t0 > ray_time.min) ray_time.min = t0;
          if(t1 < ray_time.max) ray_time.max = t1;
        }
        else
        {
          if(t1 > ray_time.min) ray_time.min = t1;
          if(t0 < ray_time.max) ray_time.max = t0;
        }

        if (ray_time.max <= ray_time.min)
        {
          return false;
        }
      }
      return true;
    }

    int longestAxis() const
    {
      // Returns the index of the longest axis of the bounding box.
      if(x.size() > y.size())
      {
        return x.size() > z.size() ? 0 : 2;
      }
      else
      {
        return y.size() > z.size() ? 1 : 2;
      }
    }

    static const AABB empty, universe;
};

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);
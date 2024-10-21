#pragma once

#include <vector>

#include "hittable.hpp"

class HittableList : public Hittable
{
  public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList(){};
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object)
    {
      objects.push_back(object);
    }

    bool hit(const Ray &ray, double ray_tmin, double ray_tmax, HitRecord &record) const override
    {
      HitRecord temp_hit_record;
      bool hit_anything = false;
      double closet_so_far = ray_tmax;

      for(const auto &object : objects)
      {
        if(object->hit(ray, ray_tmin, closet_so_far, temp_hit_record))
        {
          hit_anything = true;
          closet_so_far = temp_hit_record.t;
          record = temp_hit_record;
        }
      }
      return hit_anything;
    }
};
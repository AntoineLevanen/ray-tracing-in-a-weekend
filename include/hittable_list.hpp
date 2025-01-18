#pragma once

#include <vector>

#include "aabb.hpp"
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
      bbox = AABB(bbox, object->boundingBox());
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override
    {
      HitRecord temp_hit_record;
      bool hit_anything = false;
      auto closest_so_far = ray_t.max;

      for(const auto &object : objects)
      {
        if(object->hit(ray, Interval(ray_t.min, closest_so_far), temp_hit_record))
        {
          hit_anything = true;
          closest_so_far = temp_hit_record.t;
          record = temp_hit_record;
        }
      }
      return hit_anything;
    }

    AABB boundingBox() const override
    {
      return bbox;
    }

  private:
    AABB bbox;
};
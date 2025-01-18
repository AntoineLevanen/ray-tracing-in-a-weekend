#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include <algorithm>

class BVHNode : public Hittable
{
  public:
    BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size())
    {
      // There's a C++ subtlety here. This constructor (without span indices) create an
      // inmplicit copy of the hittable list, which we will modify. The lifetime of the copied
      // list only extends until this constructor exits. That's OK, because we only need to
      // persist the resulting bounding volume hierarchy.
    }

    BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end)
    {
      // Build the bounding box of the span of source objects.
      bbox = AABB::empty;
      for(size_t object_index=start; object_index < end; object_index++)
      {
        bbox = AABB(bbox, objects[object_index]->boundingBox());
      }

      int axis = bbox.longestAxis();

      auto comparator = (axis == 0) ? box_x_compare
                      : (axis == 1) ? box_y_compare
                      : box_z_compare;

      size_t object_span = end - start;

      if(object_span == 1)
      {
        left = right = objects[start];
      }
      else if(object_span == 2)
      {
        left = objects[start];
        right = objects[start + 1];
      }
      else
      {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<BVHNode>(objects, start, mid);
        right = make_shared<BVHNode>(objects, mid, end);
      }

    }

    bool hit(const Ray& ray, Interval ray_t, HitRecord& record) const override
    {
      if(!bbox.hit(ray, ray_t))
      {
        return false;
      }
      bool hit_left = left->hit(ray, ray_t, record);
      bool hit_right = right->hit(ray, Interval(ray_t.min, hit_left ? record.t : ray_t.max), record);

      return hit_left || hit_right;
    }

    AABB boundingBox() const override
    {
      return bbox;
    }

  private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB bbox;

    static bool boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index)
    {
      auto a_axis_interval = a->boundingBox().axisInterval(axis_index);
      auto b_axis_interval = b->boundingBox().axisInterval(axis_index);
      return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
    {
      return boxCompare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
    {
      return boxCompare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
    {
      return boxCompare(a, b, 2);
    }
};

/*
******************************
3.10
******************************
*/
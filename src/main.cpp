#include <fstream>

#include "rtweekend.hpp"

#include "bvh.hpp"
#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {

  // World

  HittableList world;

  auto material_ground = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Point3(0.0, -1000, 0.0), 1000, material_ground));

  for(int a = -11; a < 11; a++)
  {
    for(int b = -11; b < 11; b++)
    {
      auto choose_mat = randomDouble();
      Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9)
      {
        shared_ptr<Material> sphere_material;

        if(choose_mat < 0.8)
        {
          // diffuse
          auto albedo = Color::random() * Color::random();
          sphere_material = make_shared<Lambertian>(albedo);
          Vector3 center2 = center + Vector3(0, randomDouble(0.0, 0.5), 0);
          world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
        }
        else if(choose_mat < 0.95)
        {
          // metal
          auto albedo = Color::random(0.5, 1);
          auto fuzz = randomDouble(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
        else
        {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  world = HittableList(make_shared<BVHNode>(world));

  Camera camera;
  // Create a PPM image file
  std::ofstream render_image("../render/book_2.ppm");

  camera.image_width = 1600;
  camera.image_height = 800;
  camera.sample_per_pixel = 100;
  camera.max_depth = 50;

  camera.vertical_field_of_view = 20;
  camera.look_from = Point3(13, 2, 3);
  camera.look_at = Point3(0, 0, 0);
  camera.view_up = Vector3(0, 1, 0);

  camera.defocus_angle = 0.6;
  camera.focus_distance = 10.0;

  camera.render(render_image, world);

  return 0;
}
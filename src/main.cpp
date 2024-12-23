#include <fstream>

#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {

    // World

    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.75);

    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100, material_ground));  
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera camera;
    // Create a PPM image file
    std::ofstream render_image("../render/output.ppm");

    camera.image_width = 400;
    camera.image_height = 200;
    camera.sample_per_pixel = 50;
    camera.max_depth = 50;

    camera.vertical_field_of_view = 20;
    camera.look_from = Point3(-2, 2, 1);
    camera.look_at = Point3(0, 0, -1);
    camera.view_up = Vector3(0, 1, 0);

    camera.render(render_image, world);

    return 0;
}
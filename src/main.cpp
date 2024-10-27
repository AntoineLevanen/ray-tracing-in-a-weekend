#include <fstream>

#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

int main() {

    

    // World

    HittableList world;

    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));  

    Camera camera;
    // Create a PPM image file
    std::ofstream ppmFile("../render/output.ppm");

    camera.image_width = 400;
    camera.image_height = 200;
    camera.sample_per_pixel = 100;

    camera.render(ppmFile, world);

    return 0;
}
#include <iostream>
#include <fstream>

#include "color.hpp"
#include "vector3.hpp"
#include "ray.hpp"

double hit_sphere(const Point3& center, double radius, const Ray& r)
{
    Vector3 oc = center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius * radius;
    double discriminant = h*h - a*c;
    
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (h - std::sqrt(discriminant)) / a;
    }
}

Color ray_color(const Ray& r)
{
    double t = hit_sphere(Point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        Vector3 N = unit_vector(r.at(t) - Vector3(0, 0, -1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    Vector3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int image_height = 400;

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width)/image_height);
    Point3 camera_center = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vector3 viewport_u = Vector3(viewport_width, 0, 0);
    Vector3 viewport_v = Vector3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    Vector3 pixel_delta_u = viewport_u / image_width;
    Vector3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    Vector3 viewport_upper_left = camera_center - Vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    Vector3 pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Create a PPM image file
    std::ofstream ppmFile("../render/output.ppm");

    // Write the PPM header
    ppmFile << "P3" << std::endl;
    ppmFile << image_width << " " << image_height << std::endl;
    ppmFile << "255" << std::endl;

    for (int j = 0; j < image_height; j++) 
    {
        std::clog 
            << "\rScanlines remaining: " 
            << (image_height - j) 
            << ' ' << std::flush;
        
        for (int i = 0; i < image_width; i++) 
        {
            Vector3 pixel_center = pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
            Vector3 ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);


            Color pixel_color = ray_color(r);
 
            // Write RGB values to the PPM file
            write_color(ppmFile, pixel_color);
        }
        // End the line here
        ppmFile << std::endl;
    }

    // Close the PPM file
    ppmFile.close();

    std::clog << "\rDone.                 \n";

    return 0;
}
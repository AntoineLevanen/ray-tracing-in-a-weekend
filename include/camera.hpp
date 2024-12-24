#pragma once

#include <fstream>

#include "hittable.hpp"
#include "material.hpp"

class Camera
{
  public:
    // Image
    int image_width = 800; // Render image width in pixel count
    int image_height = 400; // Render image height in pixel count
    int sample_per_pixel = 10; // Count of random sample for each pixel
    int max_depth = 10; //Maximum number of ray bounces into scene

    double vertical_field_of_view = 90; // Vertical view angle (FOV)
    Point3 look_from = Point3(0, 0, 0); // Point camera is looking from
    Point3 look_at = Point3(0, 0, -1); // Point camera is looking at
    Vector3 view_up = Vector3(0, 1, 0); // Camera relative "up" direction

    double defocus_angle = 0; // Varaiation angle of rays through each pixel
    double focus_distance = 10; // Distance from camera lookfrom point to plane of perfect focus

    void render(std::ofstream &render_image, const Hittable &world)
    {
      initialize();

      // Write the PPM header
      render_image << "P3" << std::endl;
      render_image << image_width << " " << image_height << std::endl;
      render_image << "255" << std::endl;

      for (int j = 0; j < image_height; j++) 
      {
          std::clog 
              << "\rScanlines remaining: " 
              << (image_height - j) 
              << ' ' << std::flush;
          
          for (int i = 0; i < image_width; i++) 
          {
            Color pixel_color(0, 0, 0);
            for (int sample = 0; sample < sample_per_pixel; sample++)
            {
              Ray ray = getRay(i, j);
              pixel_color += rayColor(ray, max_depth, world);
            }
            
            // Write RGB values to the PPM file
            write_color(render_image, pixel_sample_scale * pixel_color);
          }
          // End the line here
          render_image << std::endl;
      }

      // Close the PPM file
      render_image.close();

      std::clog << "\rDone.                 \n";
    }

  private:
    double pixel_sample_scale; // Color scale factor for a sum of pixel samples
    Point3 camera_center;     // Camera center
    Point3 pixel00_location;  // Location of pixel 0, 0
    Vector3 pixel_delta_u;    // Offset to pixel to the right
    Vector3 pixel_delta_v;    // Offset to pixel below
    Vector3 u, v, w;          // Camera frame basis vetors
    Vector3 defocus_disk_u;   //Defocus disk horizontal radius
    Vector3 defocus_disk_v;   //Defocus disk vertical radius

    void initialize()
    {
      // Compute the pixel sample scale from the sample per pixel value.
      pixel_sample_scale = 1.0 / sample_per_pixel;

      // Camera
      double theta = deg2rad(vertical_field_of_view);
      double h = std::tan(theta / 2);
      double viewport_height = 2 * h * focus_distance;
      double viewport_width = viewport_height * (double(image_width)/image_height);
      camera_center = look_from;

      // Calculate the u, v, w unit basis vector for the camera coordinate frame.
      w = unit_vector(look_from - look_at);
      u = unit_vector(cross(view_up, w));
      v = cross(w, u);

      // Calculate the vectors across the horizontal and down the vertical viewport edges.
      Vector3 viewport_u = viewport_width * u;
      Vector3 viewport_v = viewport_height * -v;

      // Calculate the horizontal and vertical delta vectors from pixel to pixel.
      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;

      // Calculate the location of the upper left pixel.
      Vector3 viewport_upper_left = camera_center - (focus_distance * w) - viewport_u/2 - viewport_v/2;
      pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

      // Calculate the camear defocus disk basis vectors.
      double defocus_radius = focus_distance * std::tan(deg2rad(defocus_angle / 2));
      defocus_disk_u = u * defocus_radius;
      defocus_disk_v = v * defocus_radius;

    }

    Ray getRay(int i, int j) const
    {
      // Construct a camera ray originating from the defocis disk and directed at randomly sampled
      // Point around the pixel location i, j.

      Vector3 offset = sampleSquare();
      Vector3 pixel_sample = pixel00_location + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

      Point3 ray_origin = (defocus_angle <= 0) ? camera_center : defocusDiskSample();
      Vector3 ray_direction = pixel_sample - ray_origin;

      return Ray(ray_origin, ray_direction);
    }

    Vector3 sampleSquare() const
    {
      // Returns the vector to a random point in the [-0.5, -0.5]-[0.5, 0.5] unit square.
      return Vector3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
    }

    Point3 defocusDiskSample() const 
    {
      // returns a random point in the camera defocus disk.
      Point3 p = randomInUnitDisk();
      return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    Color rayColor(const Ray &ray, int depth, const Hittable &world) const
    {
      // If we have exceeded the ray bounce imit, no more is gathered.
      if (depth <= 0)
      {
        return Color(0, 0, 0);
      }

      HitRecord record;
      // Render the objects in the scene
      // Ignore hits that are very close to the calculated intersection point.
      if(world.hit(ray, Interval(0.001, infinity), record))
      {
        Ray scattered;
        Color attenuation;
        if (record.material->scatter(ray, record, attenuation, scattered))
        {
          return attenuation * rayColor(scattered, depth-1, world);
        }
        return Color(0, 0, 0);
      }

      // Render the background
      Vector3 unit_direction = unit_vector(ray.direction());
      double a = 0.5 * (unit_direction.y() + 1.0);
      return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};
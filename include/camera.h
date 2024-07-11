#pragma once

#include "utils.h"

#include "hittable.h"
#include "material.h"

class Camera
{
public:

    double aspect_ratio = 16.0 / 9.0;
    int image_width = 720;
    int samples_per_pixel = 100;
    int max_depth = 80;
    double vfov = 20;

    point3 lookfrom{ 13, 2, 3 };
    point3 lookat{ 0, 0, 0 };
    vec3 vup{ 0, 1, 0 };

    double defocus_angle = 0.3;
    double focus_distance = 13;

    void render(const hittable &world)
    {
        initialize();
        std::cout << "P3\n"
                  << image_width << ' ' << image_height << "\n255" << std::endl
                  << std::endl;

        for (int j = 0; j < image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color{0, 0, 0};
                for (size_t sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    
    int image_height;
    double pixel_samples_scale;
    point3 camera_center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u, defocus_disk_v;


    void initialize()
    {
        // Calculate the image height, and ensure that it's at least 1.
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        // Camera

        pixel_samples_scale = 1.0 / samples_per_pixel;

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
        camera_center = lookfrom;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_v = viewport_v / image_height;
        pixel_delta_u = viewport_u / image_width;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_distance * tan( degrees_to_radians(defocus_angle / 2) );
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }
    
    vec3 sample_square() const{
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    ray get_ray(int i, int j) const
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        return ray(ray_origin, pixel_sample - ray_origin);
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        if (depth <= 0)
            return color(0, 0, 0);
        
        hit_record rec;
        if (world.hit(r, {0.001, kInfinity}, rec))
        {   
            ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    point3 defocus_disk_sample() const
    {
        auto p = random_in_unit_disk();
        return camera_center + (p.x() * defocus_disk_u + p.y() * defocus_disk_v);
    }
};
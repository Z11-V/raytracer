#pragma once

#include "hittable.hpp"
#include "material.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class camera
{
private:
    int image_height;
    point3 camera_center;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    point3 pixel00_loc; // location of pixel 0,0
    unsigned char* image;
    vec3 u, v, w; // camera basis vector

    void initialize()
    {
        image_height = int( image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; //ensure it's atleast 1

        image = (unsigned char*) malloc(image_width * image_height * 3);

        camera_center = lookfrom;

        // Viewport
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = h * focal_length;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up,w));
        v= cross(w,u);
        
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    vec3 sample_square() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    ray get_ray(int i, int j) const
    {
        auto offset = sample_square();
        auto pixel_sample_center = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        auto ray_direction = pixel_sample_center - camera_center;
        return ray(camera_center, ray_direction);
    }

    color environmental_light(const ray&r) const
    {
        // background
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        auto startcolor = color(1.0, 1.0, 1.0);
        auto endcolor = color(0.5, 0.7, 1.0);
        return lerp(startcolor, endcolor, a);
    }

    color ray_color(const ray& r, int depth ,const hittable& world) const
    {
        if (depth <= 0)
        {
            return color(0,0,0);
        }

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            color emission_color = rec.mat->emitted(rec.u, rec.v, rec.p);
            if (rec.mat->scatter(r, rec, attenuation, scattered))
            {
                color scatter_color = attenuation * ray_color(scattered, depth-1, world);
                return emission_color + scatter_color;
            }
            // no scatter ray
            return emission_color;
        }
        // no hit
        //return environmental_light(r);
        return color(0,0,0);
    }

public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    int max_bounces = 10;
    double vfov = 60; //vertical view angle

    point3 lookfrom = point3(0,0,0);
    point3 lookat = point3(0,0,-1);
    vec3 up = vec3(0,1,0); // Camera up
 
    void render(const hittable& world)
    {
        initialize();
        for (size_t j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (size_t i = 0; i < image_width; i++)
            {
                color pixel_color(0,0,0);
                for (size_t sample = 0; sample < samples_per_pixel; sample++)
                {
                    auto r = get_ray(i, j);
                    pixel_color += ray_color(r,max_bounces,world);
                }
                write_color(image, pixel_color * (1.0 / samples_per_pixel));
            } 
        }
        stbi_write_png("output/test.png", image_width, image_height, 3 , (void*)image, image_width*3);
        free(image);

        std::clog << "\rDone.                    \n";
    }

};
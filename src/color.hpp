#pragma once

#include "interval.hpp"
#include "vec3.hpp"

using color = vec3;

double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrt(linear_component);
    }
    return 0;
}

void write_color(unsigned char* image, const color& pixel_color)
{
    static int pixels_written = 0;

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // map [0,1] values to [0,255]
    static const interval intensity(0.0, 0.999);
    int rbyte = int(256*intensity.clamp(r));
    int gbyte = int(256*intensity.clamp(g));
    int bbyte = int(256*intensity.clamp(b));

    image[pixels_written*3 + 0] = rbyte;
    image[pixels_written*3 + 1] = gbyte;
    image[pixels_written*3 + 2] = bbyte;

    pixels_written++;
}
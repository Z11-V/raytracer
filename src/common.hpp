#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

double random_double()
{
    // returns a random real in [0,1).
    return std::rand() / (RAND_MAX+1.0);
}

double random_double(double min, double max)
{
    // returns a random real in [min,max).
    return min + (max - min) * random_double();
}

// Common headers

#include "interval.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"
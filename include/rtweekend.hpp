#pragma once

// Standard Headers

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

// C++ Std Usings

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double deg2rad(double degrees)
{
    return degrees * pi / 180.0;
}

inline double randomDouble()
{
    // Return a random real in [0, 1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max)
{
    // Return a random real in [min, max).
    return min + (max - min) * randomDouble();
}

// Common Headers
// They must be included here since they use  the infinity and py constante.

#include "color.hpp"
#include "ray.hpp"
#include "vector3.hpp"
#include "interval.hpp"
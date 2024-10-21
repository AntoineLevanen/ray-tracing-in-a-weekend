#pragma once

// Standard Headers

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// Common Headers

#include "color.hpp"
#include "ray.hpp"
#include "vector3.hpp"

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
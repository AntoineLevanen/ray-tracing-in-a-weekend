cmake_minimum_required(VERSION 3.15)

project(RayTracerInOneWeekend VERSION 0.1.0 LANGUAGES C CXX)

add_executable(RayTracerInOneWeekend src/main.cpp) # include/vector3.hpp include/color.hpp include/ray.hpp)

include_directories(include)

set_property(TARGET RayTracerInOneWeekend PROPERTY CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.15)

project(RayTracerInOneWeekend VERSION 0.1.0 LANGUAGES C CXX)

add_executable(RayTracerInOneWeekend src/main.cpp)

set_property(TARGET RayTracerInOneWeekend PROPERTY CXX_STANDARD 17)
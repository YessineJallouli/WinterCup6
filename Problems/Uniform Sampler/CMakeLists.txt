cmake_minimum_required(VERSION 3.26)
project(UniformSampler)

set(CMAKE_CXX_STANDARD 23)

add_executable(UniformSampler solution.cpp)
add_executable(UniformSamplerGenerator generator.cpp)

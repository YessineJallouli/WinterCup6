cmake_minimum_required(VERSION 3.26)
project(ModularUniverse)

set(CMAKE_CXX_STANDARD 20)

add_executable(ModularUniverse solution.cpp)
add_executable(ModularUniverseGenerator generator.cpp)

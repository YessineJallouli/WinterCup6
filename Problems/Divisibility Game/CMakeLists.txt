cmake_minimum_required(VERSION 3.26)
project(DivisibilityGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(Divisibility solution.cpp)
add_executable(CDivisibility solution.c)
add_executable(DivisibilityGrundy grundy.cpp)
add_executable(DivisibilityGenerator generator.cpp)
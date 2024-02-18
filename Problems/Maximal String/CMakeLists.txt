cmake_minimum_required(VERSION 3.26)
project(MaximalString)

set(CMAKE_CXX_STANDARD 20)

add_executable(MaximalString solution.cpp)
add_executable(CMaximalString solution.c)
add_executable(CMaximalStringNaive naive.c)

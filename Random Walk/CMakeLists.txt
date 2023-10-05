cmake_minimum_required(VERSION 3.26)
project(Random_Walk)

set(CMAKE_CXX_STANDARD 17)

add_executable(RandomWalkMonteCarlo MC.cpp)
add_executable(RandomWalkSolution solution.cpp)

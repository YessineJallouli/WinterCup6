cmake_minimum_required(VERSION 3.11)

project(BalancedProblemset)
#Put this in your CMakeLists.txt file (change any options from OFF to ON if you want):

set(CMAKE_CXX_STANDARD 20)
add_executable(BalancedProblemset solution.cpp)
add_executable(CBalancedProblemset solution.c)
add_executable(BalancedProblemsetGen generator.cpp)

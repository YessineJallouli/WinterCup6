cmake_minimum_required(VERSION 3.26)
project(InfiniteMoney)

set(CMAKE_CXX_STANDARD 20)

add_executable(HideTheMoney solution.cpp)
add_executable(HideTheMoneyOpt solution_optimised.cpp)

add_executable(HideTheMoneyBool solution_bool.cpp)
add_executable(HideTheMoneyBoolOpt solution_bool_opt.cpp)

add_executable(CHideTheMoney solution.c)

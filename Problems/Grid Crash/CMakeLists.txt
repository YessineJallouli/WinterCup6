cmake_minimum_required(VERSION 3.26)
project(GridCrash)

set(CMAKE_CXX_STANDARD 20)
add_executable(GridCrashSolution solution.cpp)
add_executable(CGridCrashSolution solution.c)
add_executable(GridCrashBFS solution.cpp)
add_executable(GridCrashHeuristic solution.cpp)
add_executable(GridCrashCombined solution.cpp)
add_executable(GridCrashBinarySolution binary.cpp)
add_executable(GridCrashBinaryBFS binary.cpp)
add_executable(GridCrashBinaryHeuristic binary.cpp)
add_executable(GridCrashBinaryCombined binary.cpp)
add_executable(GridCrashMTaylor mtaylor.cpp)
target_compile_definitions(GridCrashSolution PUBLIC TIMER_ON)
target_compile_definitions(GridCrashBFS PUBLIC TIMER_ON)
target_compile_definitions(GridCrashHeuristic PUBLIC TIMER_ON)
target_compile_definitions(GridCrashCombined PUBLIC TIMER_ON)
target_compile_definitions(GridCrashSolution PUBLIC COLOUR_HEURISTIC)
target_compile_definitions(GridCrashHeuristic PUBLIC CLUSTER_HEURISTIC)
target_compile_definitions(GridCrashCombined PUBLIC COMBINED_HEURISTIC)
target_compile_definitions(GridCrashBinarySolution PUBLIC COLOUR_HEURISTIC)
target_compile_definitions(GridCrashBinaryHeuristic PUBLIC CLUSTER_HEURISTIC)
target_compile_definitions(GridCrashBinaryCombined PUBLIC COMBINED_HEURISTIC)

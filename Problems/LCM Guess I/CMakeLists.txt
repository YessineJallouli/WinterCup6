cmake_minimum_required(VERSION 3.26)
project(Guess_the_array)

set(CMAKE_CXX_STANDARD 20)


add_executable(GuessInteractor interactor.cpp)
add_executable(GuessGenerator generator.cpp)
add_executable(GuessGenerator2 generator_2.cpp)
add_executable(GuessChecker checker.cpp)
add_executable(GuessSolution solution.cpp)
add_executable(GuessSolutionOne find_one.cpp)
add_executable(GuessSolutionIterative iterative.cpp)
add_executable(GuessSolutionPrime largest_prime.cpp)
add_executable(GuessSolutionHeuristic large_prime.cpp)
add_executable(CGuessSolution solution.c)
add_executable(GuessCInteractor attachments/src/interactor.c)
add_executable(GuessCppInteractor attachments/src/interactor.cpp)
set_property(TARGET GuessCppInteractor PROPERTY CXX_STANDARD 11)

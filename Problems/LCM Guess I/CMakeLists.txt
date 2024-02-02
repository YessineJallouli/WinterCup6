cmake_minimum_required(VERSION 3.26)
project(Guess_the_array)

set(CMAKE_CXX_STANDARD 20)


add_executable(GuessInteractor interactor.cpp)
add_executable(GuessGenerator generator.cpp)
add_executable(GuessChecker checker.cpp)
add_executable(GuessSolution solution.cpp)
add_executable(GuessSolutionOne find_one.cpp)
add_executable(GuessSolutionPrime largest_prime.cpp)
add_executable(GuessSolutionHeuristic large_prime.cpp)

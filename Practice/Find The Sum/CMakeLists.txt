cmake_minimum_required(VERSION 3.26)
project(Find_Sum)

set(CMAKE_CXX_STANDARD 20)


add_executable(FindSum solution.cpp)
add_executable(FindSumCInteractor attachments/src/interactor.c)
add_executable(FindSumCppInteractor attachments/src/interactor.cpp)
set_property(TARGET FindSumCppInteractor PROPERTY CXX_STANDARD 11)

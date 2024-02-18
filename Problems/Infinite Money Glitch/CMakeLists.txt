cmake_minimum_required(VERSION 3.26)
project(InfiniteMoney)

set(CMAKE_CXX_STANDARD 23)

add_executable(InfiniteMoney solution.cpp)
add_executable(InfiniteMoneyGenerator generator.cpp)
add_executable(InfiniteMoneyValidator validator.cpp)
add_executable(CInfiniteMoney solution.c)
target_link_libraries(CInfiniteMoney m)

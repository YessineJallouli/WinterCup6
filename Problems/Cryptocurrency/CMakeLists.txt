cmake_minimum_required(VERSION 3.11)

project(Cryptocurrency)
#Put this in your CMakeLists.txt file (change any options from OFF to ON if you want):

set(CMAKE_CXX_STANDARD 20)
add_executable(CCryptocurrency solution.c)
add_executable(CryptocurrencyGenerator generator.cpp)

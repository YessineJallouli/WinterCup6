cmake_minimum_required(VERSION 3.11)


#Put this in your CMakeLists.txt file (change any options from OFF to ON if you want):

set(CMAKE_CXX_STANDARD 20)
add_executable(SectorColoursV1 version1.cpp)
add_executable(CSectorColours solution.c)
#add_executable(SectorColoursV2 version2.cpp)
add_executable(SectorColoursGenerator generator.cpp)
target_link_libraries(CSectorColours m)

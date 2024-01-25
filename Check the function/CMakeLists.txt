cmake_minimum_required(VERSION 3.26)
project(Check_the_function)

set(CMAKE_CXX_STANDARD 17)

add_executable(solution main.cpp)
#add_executable(brute_force n*n_solution.cpp)
add_executable(generatorYes generate_all_yes.cpp)
add_executable(generateYesNo generate_yes_no.cpp)
add_executable(generate_yes generate_yes.cpp)
add_executable(generate_no generate_no.cpp)
add_executable(mtaylor mtaylor_main_solution.cpp)
add_executable(checker checker.cpp)
add_executable(constructive_solution constructive_solution.cpp)
cmake_minimum_required(VERSION 3.16.3)
project(test_control)

SET(CMAKE_VERBOSE_MAKEFILE ON)
#include_directories(${CMAKE_SOURCE_DIR}/lib/nlohmann)
message("OS: ${CMAKE_SYSTEM_NAME}")

find_package(flexiv_rdk REQUIRED)
add_executable(demo test_control.cpp)
add_executable(sine sine_control_demo.cpp)
target_link_libraries(demo flexiv::flexiv_rdk)
target_link_libraries(sine flexiv::flexiv_rdk)
set_target_properties(demo PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)

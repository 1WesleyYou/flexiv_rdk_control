cmake_minimum_required(VERSION 3.16.3)
project(test_control)

SET(CMAKE_VERBOSE_MAKEFILE ON)
#include_directories(${CMAKE_SOURCE_DIR}/lib/nlohmann)
message("OS: ${CMAKE_SYSTEM_NAME}")

find_package(flexiv_rdk REQUIRED)
add_executable(demo test_control.cpp)
add_executable(sine base/sine_control_demo.cpp)
add_executable(test_every_joint base/test_every_joint.cpp)
add_executable(gripper base/gripper_test.cpp)
target_link_libraries(demo flexiv::flexiv_rdk)
target_link_libraries(sine flexiv::flexiv_rdk)
target_link_libraries(test_every_joint flexiv::flexiv_rdk)
target_link_libraries(gripper flexiv::flexiv_rdk)
set_target_properties(demo PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)

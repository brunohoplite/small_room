cmake_minimum_required(VERSION 3.10)

# Project name
project(small_room)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define source files in a variable
set(SOURCES
    src/main.cpp
    src/sysfs_pwm.cpp
    src/control_box.cpp
    src/presence_detector.cpp
    src/timer.hpp
)

# Add the executable
add_executable(small_room ${SOURCES})

# Add compiler flags
target_compile_options(small_room PRIVATE
    -Wall
    -Wextra
    -Werror
)

find_package(Crow)

# Link with wiringPi library
target_link_libraries(small_room wiringPi Crow::Crow)

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
)

# Add the executable
add_executable(small_room ${SOURCES})

# Link with wiringPi library
target_link_libraries(small_room wiringPi)

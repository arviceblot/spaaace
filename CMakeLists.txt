cmake_minimum_required(VERSION 2.8)
project(spaaace)

# Add C++11 feature support
add_definitions(-std=c++11)

# Add appropriate debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")

find_package(SDL REQUIRED sdl sdl_opengl)
find_package(OpenGL REQUIRED)

# Add local code
add_subdirectory(src)

cmake_minimum_required(VERSION 3.22)

project(go-dotter)

# option(ARCHITECTURE "x64 or x86 Architecture" "x64")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(godot-cpp)
add_subdirectory(example-plugin)
# add other cool plugins here
# |
# v
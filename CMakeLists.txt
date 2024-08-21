cmake_minimum_required(VERSION 3.22)

project(go-dotter)

option(EXPORT "Exporting game for publishing" OFF)
option(EXPORT_NON_RELOADABLE "Only export non-reloadable when executable is not running" OFF)
option(ENABLE_DD3D "Enable or disable DD3D debug rendering" ON)

# option(ARCHITECTURE "x64 or x86 Architecture" "x64")
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(godot-cpp)
add_subdirectory(example-plugin)

# add other cool plugins here
# |
# v
add_subdirectory(debug-draw-3d-plugin)
add_subdirectory(gameplay-plugin)
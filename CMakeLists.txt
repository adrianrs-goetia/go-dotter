cmake_minimum_required(VERSION 3.22)

project(go-dotter)

option(ARCHITECTURE "x64 or x86 Architecture" "x64")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${ARCHITECTURE}")
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(godot-cpp) # required to provide your own link or copy to repo
add_subdirectory(TrialPlugin)
# add other cool plugins here
# |
# v
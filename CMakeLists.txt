
cmake_minimum_required(VERSION 2.8)
add_definitions("-Wall -std=c++11")
add_subdirectory(Structure)
add_executable(StructureTest main.cpp)
target_link_libraries(StructureTest Structure-Mod)



cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++1y")
add_subdirectory(Structure)
add_executable(StructureTest main.cpp)
target_link_libraries(StructureTest Structure-Mod)


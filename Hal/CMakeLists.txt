cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++1y")

# ファイルをライブラリとして追加
file(GLOB HAL_SOURCES *.cpp *.h *.hpp)
add_library(hal ${HAL_SOURCES})

# インクルードディレクトリ追加
target_include_directories(hal PUBLIC .)

add_subdirectory(Heuristics)
add_subdirectory(Solver)
add_subdirectory(Perfect)
add_subdirectory(NetWork)

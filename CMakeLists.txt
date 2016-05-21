
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
PROJECT(Procon26 CXX)

# マクロ設定
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")        # C++14を有効化
add_definitions(-DMATRIX_WIDEOUT)

# in-sourceビルドの禁止
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-sourceビルドは許可されていません.")
endif()

# 出力先を設定
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for executables")
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for libraries")


# サブディレクトリ追加
add_subdirectory(Structure)
add_subdirectory(Utility)
add_subdirectory(Hal)
add_subdirectory(Riya)

# 実行ファイル作成
add_executable(Solver Hal/main.cpp)
target_link_libraries(Solver structure)
target_link_libraries(Solver hal)
target_link_libraries(Solver riya)
target_link_libraries(Solver heuristic)
target_link_libraries(Solver perfect)
target_link_libraries(Solver solver)
# target_link_libraries(Solver network)




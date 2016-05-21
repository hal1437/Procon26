
cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++1y")

# ファイルをライブラリとして追加
file(GLOB NETWORK_SOURCES *.cpp *.h *.hpp)
add_library(network ${NETWORK_SOURCES})

# インクルードディレクトリ追加
target_include_directories(network PUBLIC .)



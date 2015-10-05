
cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++1y")
add_library(Structure-Mod STATIC
	Transform.cpp
	Heuristic.cpp
	Problem.cpp
	Answer.cpp
	Point.cpp 
)



cmake_minimum_required(VERSION 2.8)
add_definitions("-Wall -std=c++11") # <= new
add_executable(SutructureTest test.cpp
	test.cpp
	Transform.cpp
	Heuristic.cpp
	Problem.cpp
	Answer.cpp
	Point.cpp 
)


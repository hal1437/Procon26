cmake_minimum_required(VERSION 2.8)
add_definitions("-Wall -std=c++11")
add_executable(Procon26-Hal
  main.cpp
  Solver/BestFirst.cpp
  Solver/DLS.cpp
  Heuristics/Cavity.cpp
  Heuristics/DensityAround.cpp
  Heuristics/WeightComposit.cpp
  Heuristics/Density.cpp
  Heuristics/Score.cpp
  ../Structure/Transform.cpp
  ../Structure/Point.cpp
  ../Structure/Heuristic.cpp
  ../Structure/Problem.cpp	
  ../Structure/Answer.cpp
)

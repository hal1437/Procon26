cmake_minimum_required(VERSION 2.8)
add_definitions("-O0 -std=c++1y")
add_executable(Procon26-Hal
  main.cpp
  Solver/BestFirst.cpp
  Solver/DLS.cpp
  Solver/PerfectBackTrack.cpp
  Heuristics/Cavity.cpp
  Heuristics/DensityAround.cpp
  Heuristics/WeightComposit.cpp
  Heuristics/Density.cpp
  Heuristics/Score.cpp
  Perfect/BlockSize.cpp
  Perfect/ParityCheck.cpp
  Perfect/DPBlockSize.cpp
  Perfect/PerfectComposit.cpp
  ../Structure/Transform.cpp
  ../Structure/Point.cpp
  ../Structure/Heuristic.cpp
  ../Structure/Problem.cpp	
  ../Structure/Answer.cpp
)


#include <iostream>
#include <fstream>
#include <random>
#include "Structure/Problem.h"
#include "Structure/Answer.h"
#include "Structure/Solver.h"
#include "Structure/Matrix.hpp"
#include "Utility/BenchMark.hpp"



int main(){

	typedef MultiBit<4,4> M;
	constexpr MultiBit<4,4> bits = MultiBit<4,4>({{1,1,1,1},
												  {1,0,0,1},
												  {1,0,0,1},
												  {1,1,1,1}});
	constexpr Matrix<4,4> mat   = Matrix<4,4>(bits);
	constexpr Matrix<4,4> mat_r = Matrix<4,4>(mat.GetReverse(true));

	std::cout << mat << std::endl;
	
	return 0;
}

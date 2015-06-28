
#include <iostream>
#include <fstream>
#include <random>
#include "Structure/Problem.h"
#include "Structure/Answer.h"
#include "Structure/Solver.h"
#include "Structure/Matrix.hpp"
#include "Utility/BenchMark.hpp"


int main(){

	constexpr Matrix<4,4> bits  = {{1,1,1,0},
								   {1,0,0,0},
								   {1,0,0,1},
								   {1,1,1,1}};
	constexpr Matrix<4,4> bits2 = {{0,0,0,0},
								   {0,1,1,0},
								   {0,1,1,0},
								   {0,0,0,0}};

	constexpr Matrix<4,4> mat   = Matrix<4,4>(bits);
	constexpr Matrix<4,4> mat_r = Matrix<4,4>(mat.GetRotate(Constants::ANGLE90));
	constexpr Matrix<4,4> mat_p = Matrix<4,4>(mat.GetProjection(bits2));
	constexpr size_t hash = mat_p.to_hash();
	//constexpr Matrix<4,4> m = Matrix<4,4>(hash);

	
	std::cout << mat_p << std::endl;
	std::cout << hash << std::endl;
	std::cout << Matrix<4,4>(hash) << std::endl;
	
	return 0;
}

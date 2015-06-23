
#include <iostream>
#include <fstream>
#include <random>
#include "Structure/Problem.h"
#include "Structure/Answer.h"
#include "Structure/Solver.h"
#include "Structure/Matrix.hpp"
#include "Utility/BenchMark.hpp"

int main(){

	Problem prob("Problem/quest9.txt");
	std::ofstream ofs("Answer.txt");
	Field field;// = prob.GetField();
	Answer ans(prob);
	
	Matrix<3,3> mat;
	mat[1][0]=1;
	mat[1][1]=1;
	mat[0][0]=1;
	mat[2][2]=1;
	std::cout << mat << std::endl;
	std::cout << mat.GetNormalize() << std::endl;

	//===========BENCHMARK RESULT===========
	//    [COUNT]          500 times
	//[FULL TIME]         9756 msec
	// [PER TIME]       19.512 msec/function 
	//======================================
	/*
	std::cout << prob.GetField() << std::endl;
	BenchMark<500>()([&](){
		prob.GetField().GetListLayPossible(prob.GetBlock(0));
	});
	*/
	//std::cout << prob.GetField() << std::endl;
	
	return 0;
}

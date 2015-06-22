
#include <iostream>
#include <fstream>
#include <random>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Matrix.hpp"
#include "../Utility/BenchMark.hpp"

int main(){

	Problem prob("../Problem/quest9.txt");
	std::ofstream ofs("Answer.txt");
	Field field;// = prob.GetField();
	Answer ans(prob);
	
	//===========BENCHMARK RESULT===========
	//    [COUNT]          500 times
	//[FULL TIME]         9756 msec
	// [PER TIME]       19.512 msec/function 
	//======================================
	
	std::cout << prob.GetField() << std::endl;
	BenchMark<500>()([&](){
		prob.GetField().GetListLayPossible(prob.GetBlock(0));
	});
	//std::cout << prob.GetField() << std::endl;
	
	return 0;
}

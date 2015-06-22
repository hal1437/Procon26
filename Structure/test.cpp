
#include <iostream>
#include <fstream>
#include <random>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Matrix.hpp"
#include "../Utility/BenchMark.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	std::ofstream ofs("Answer.txt");
	Field field;// = prob.GetField();
	Answer ans(prob);

	ans.SetTransform(0,Transform(Point(0,0),Constants::ANGLE0,false));
	ans.SetTransform(1,Transform(Point(0,0),Constants::ANGLE0,false));
	ans.SetTransform(2,Transform(Point(0,0),Constants::ANGLE0,false));
	ans.SetTransform(3,Transform(Point(0,0),Constants::ANGLE0,false));
	std::cout << prob << std::endl;
	std::cout << ans << std::endl;

	return 0;
}

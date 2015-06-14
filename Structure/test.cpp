
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Matrix.hpp"
#include "../Utility/BenchMark.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	std::ofstream ofs("Answer.txt");
	Field field = prob.GetField();
	Answer ans(prob);
	Solver* solv;
	Block multi;

	std::cout << field << std::endl;
	field.Projection(prob.GetBlock(0),Transform(Point(-1,0),Constants::ANGLE0,false));
	std::cout << field << std::endl;
	field.ReverseProjection(prob.GetBlock(0),Transform(Point(-1,0),Constants::ANGLE0,false));
	std::cout << field << std::endl;

	/*
	field.Projection(prob.GetBlock(0),Transform(Point(1,0),Constants::ANGLE0 ,false));
	std::cout << field << std::endl;
	
	for(Transform f: Field(field ^ prob.GetField()).GetListLayPossible(prob.GetBlock(1))){
		std::cout << f << std::endl;
		std::cout << (field | prob.GetBlock(1).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(f)) << std::endl;
	}
	*/

	Transform first_trans(Point(3,2),Constants::ANGLE0,false);
	
	return 0;
}

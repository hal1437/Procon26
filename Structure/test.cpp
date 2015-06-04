
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Matrix.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	std::ofstream ofs("Answer.txt");
	Field field = prob.GetField();
	Answer ans(prob);
	Solver* solv;
	Block multi;

	std::cout << std::boolalpha;
	ofs << std::boolalpha;

	std::cout << field << std::endl;
	for(int i=0;i<prob.Count();i++){
		std::cout << prob.GetBlock(i) << std::endl;
	}
	
	field.Projection(prob.GetBlock(0),Transform(Point( 3,2),Constants::ANGLE0 ,false));
	field.Projection(prob.GetBlock(1),Transform(Point(-2,-3),Constants::ANGLE90,true));

	field.Projection(prob.GetBlock(3),Transform(Point(6,0),Constants::ANGLE270,false));
	
	std::cout << field << std::endl;

/*

	//std::cout << prob.GetBlock(1).count() << std::endl;
	//std::cout << (Block() != Block()) << std::endl;
	
	for(int i=0;i<255;i++){
		//std::vector<Transform> hands = field.GetListLayPossible(prob.GetBlock(i));
		//for(const Transform& hand:hands){
			Field _field = field;
			
			//_field.Projection(hand);
			//ofs << hand << std::endl;
			std::cout << _field.Move(Point(2,3)) << std::endl;
			break;
		//}
	}
	
	//std::cout << ans;
*/
	return 0;
}


#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "MultiBit.hpp"

int main(){

	Problem prob("../Problem/quest1.txt");
	std::ofstream ofs("output.txt");
	Field field = prob.GetField();
	Answer ans;
	Solver* solv;
	Block multi;

	std::cout << std::boolalpha;
	ofs << std::boolalpha;

	field.Projection(Point(2,1),prob.GetBlock(0));
	std::cout << field;
	/*
	for(int i=0;i<255;i++){
		std::vector<Hand> hands = field.GetListLayPossible(prob.GetBlock(i));
		for(const Hand& hand:hands){
			Field _field = field;
			
			_field.Projection(hand);
			ofs << hand.pos << "," << hand.angle << "," << hand.reverse << std::endl;
			ofs << hand.block << std::endl;
			ofs << _field << std::endl;
			break;
		}
	}*/

	return 0;
}

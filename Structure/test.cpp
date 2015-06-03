
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "MultiBit.hpp"
#include "Matrix.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	std::ofstream ofs("Answer.txt");
	Field field = prob.GetField();
	Answer ans;
	Solver* solv;
	Block multi;

	std::cout << std::boolalpha;
	ofs << std::boolalpha;

	//std::cout << prob.GetBlock(1).count() << std::endl;
	//std::cout << (Block() != Block()) << std::endl;
	/*
	for(int i=0;i<255;i++){
		std::vector<Hand> hands = field.GetListLayPossible(prob.GetBlock(i));
		for(const Hand& hand:hands){
			Field _field = field;
			
			_field.Projection(hand);
			ofs << hand << std::endl;
			ofs << _field << std::endl;
			break;
		}
	}
	*/
	std::cout << ans;

	return 0;
}

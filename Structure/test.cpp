
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "MultiBit.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	Answer ans;
	Solver* solv;
	Block multi;

	std::cout << std::boolalpha;
	//multi[0][0] = true;

	//std::cout << prob.GetField().isLayPossible(Point(-7,-7),multi.GetRotate(Constants::ANGLE180)) << std::endl;

	//std::cout << multi;

	//ans.SetField(prob.GetField());
	//ans.AddBlocks(prob.GetBlock(0),Point(1,1),false,Constants::ANGLE90);

	//std::cout << ans.GetField()  << std::endl;
	//std::cout << ans             << std::endl;

	//std::cout << prob.GetBlock(0).GetRotate(Constants::ANGLE180);
	//std::cout << prob.get() << std::endl;

	//std::cout << multi;

	std::ofstream ofs("output.txt");
	std::vector<Hand> hands;

	ofs << std::boolalpha;
	hands = prob.GetField().GetListLayPossible(prob.GetBlock(0));
	for(Hand& hand:hands){
		Field field = prob.GetField();
		
		field.Projection(hand);
		ofs << hand.pos << "," << hand.angle << "," << hand.reverse << std::endl;
		ofs << hand.block << std::endl;
		ofs << field << std::endl;
	}

	return 0;
}

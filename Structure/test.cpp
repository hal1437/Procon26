
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
	MultiBit<8,8> multi;

	//std::cout << multi;

	ans.SetField(prob.GetField());
	ans.AddBlocks(prob.GetBlock(0),Point(1,1),false,Constants::ANGLE90);

	//std::cout << prob.GetField() << std::endl;
	//std::cout << ans.GetField()  << std::endl;
	//std::cout << ans             << std::endl;

	std::ofstream ofs("output.txt");
	std::vector<Hand> hands;
	hands = prob.GetField().GetListLayPossible(prob.GetBlock(0));
	for(Hand& hand:hands){
		Field field = prob.GetField();
		Block block;
		if(hand.reverse)block = prob.GetBlock(0).GetReverse();
		else block = prob.GetBlock(0);
		field.Projection(hand.pos,block);
		ofs << hand.pos << std::endl;
		ofs << field    << std::endl;
	}

	return 0;
}

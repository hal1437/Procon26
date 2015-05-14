
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"

int main(){

	Problem prob("../Problem/Problem.txt");
	Answer ans;

	ans.SetField(prob.GetField());
	ans.AddBlocks(prob.GetBlock(0),Point(1,1),false,Constants::ANGLE90);

	//Field field = prob.GetField();
	//Block block = prob.GetBlock(0);

	//std::cout << block << std::endl;
	//std::cout << "isLayPossible:" << field.isLayPossible(Point(3,0),block.GetRotate(Constants::ANGLE90)) << std::endl;
	//field.Projection(Point(3,0),block.GetRotate(Constants::ANGLE90));
	std::cout << prob.GetField() << std::endl;
	std::cout << ans.GetField()  << std::endl;
	std::cout << ans             << std::endl;


	return 0;
}

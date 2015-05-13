
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"

int main(){

	Problem prob("../Problem/Problem.txt");

	Field field = prob.GetField();
	Block block = prob.GetBlock(0);

	std::cout << block << std::endl;
	std::cout << "isLayPossible:" << field.isLayPossible(Point(3,0),block.GetRotate(Constants::ANGLE90)) << std::endl;
	field.Projection(Point(3,0),block.GetRotate(Constants::ANGLE90));
	std::cout << field << std::endl;

	return 0;
}

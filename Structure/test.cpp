
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>

int main(){

	
	
	Field field;
	Block block;

	std::ifstream ffs("out.txt");
	std::ifstream bfs("block.txt");

	ffs >> field;
	bfs >> block;

	std::cout << field << std::endl;
	std::cout << block << std::endl;
	std::cout << "isLayPossible:" << field.isLayPossible(Point(-1,-1),block.GetRotate(Constants::ANGLE90)) << std::endl;
	field.Projection(Point(3,0),block.GetRotate(Constants::ANGLE90));
	std::cout << field << std::endl;

	return 0;
}

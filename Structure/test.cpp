
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

	std::cout << prob.GetField() << std::endl;
	std::cout << ans.GetField()  << std::endl;
	std::cout << ans             << std::endl;

	return 0;
}

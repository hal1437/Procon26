#include "Block.h"
#include <iostream>

int main(){
	Block b;
	
	b[1][1] = Constants::FILL;
	b[2][1] = Constants::FILL;
	b[2][2] = Constants::FILL;

	std::cout << b << std::endl;
	std::cout << b.GetRotate(Constants::ANGLE90) << std::endl;
	std::cout << b.GetRotate(Constants::ANGLE180) << std::endl;
	std::cout << b.GetRotate(Constants::ANGLE270) << std::endl;
	std::cout << b.GetReverse() << std::endl;

	return 0;
}

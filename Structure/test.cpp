#include "Block.h"
#include <iostream>

int main(){
	Block b;
	
	b[1][1] = USED;
	b[2][1] = USED;
	b[2][2] = USED;
	
	std::cout << b << std::endl;
	std::cout << b.GetRotate(ANGLE90) << std::endl;
	std::cout << b.GetRotate(ANGLE180) << std::endl;
	std::cout << b.GetRotate(ANGLE270) << std::endl;
	std::cout << b.GetReverse() << std::endl;

	return 0;
}

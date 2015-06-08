
#include "Block.h"
#include "Field.h"
#include <iostream>
#include "../Utility/CLOCKWISE_FOR.hpp"
#include "../Utility/BenchMark.hpp"

int main(){
	BenchMark<400>()([]{
		for(int i=0;i<10000000;i++);
	});
	
	//std::cout << field << std::endl;
	//std::cout << b.GetRotate(Constants::ANGLE90) << std::endl;
	//std::cout << b.GetRotate(Constants::ANGLE180) << std::endl;
	//std::cout << b.GetRotate(Constants::ANGLE270) << std::endl;
	//std::cout << b.GetReverse() << std::endl;

	return 0;
}

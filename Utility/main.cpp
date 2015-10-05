
#include <iostream>
#include "Quickout.hpp"
#include "Timewait.hpp"

int main(){
	
	std::cout << "Hello" << "World";
	Timewait(3000);
	
	Qo("Hello","World");
	return 0;
}

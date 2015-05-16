
#include <iostream>
#include "../Structure/Problem.h"
#include "Heuristics/Density.h"

int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/Problem.txt");

	Heuristics<unsigned,Field>* density;
	density = new Density();
	std::cout << prob.GetField();
	std::cout << "Density Heuristics:" << density->Execution(prob.GetField()) << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

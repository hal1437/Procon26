
#include <iostream>
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Solver/AStar.h"
#include "Solver/BestFirst.h"

int main(){

	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/Problem.txt");
	Solver* solver;
	Heuristics<unsigned,Field>* density;

	density = new DensityAround();
	solver  = new BestFirst(prob,density);
	std::cout << prob.GetField();
	std::cout << "Density Heuristics:" << density->Execution(prob.GetField()) << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;

}


#include <iostream>
#include <fstream>
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Solver/AStar.h"
#include "Solver/BestFirst.h"

int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest5.txt");
	std::ofstream ofs("Answer.txt");
	Solver* solver;
	Heuristics<unsigned,Field>* density;

	density = new DensityAround();
	solver  = new BestFirst(prob,density);
	
	ofs << solver->Solve() << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

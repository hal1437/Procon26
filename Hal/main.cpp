
#include <iostream>
#include <fstream>
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/Cavity.h"
#include "Heuristics/WeightComposit.h"
#include "Solver/AStar.h"
#include "Solver/BestFirst.h"

int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest1.txt");
	std::ofstream ofs("Answer.txt");
	Solver* solver;
	WeightComposit* density;

	density = new WeightComposit();
	solver  = new BestFirst(prob,density);
	
	density->AddHeuristic(new DensityAround(),1.0f);
	density->AddHeuristic(new Cavity(),8.0f);

	ofs << solver->Solve() << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

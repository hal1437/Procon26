
#include <iostream>
#include <fstream>
#include "../Structure/Problem.h"
#include "Heuristics/Score.h"
#include "Heuristics/Density.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/Cavity.h"
#include "Heuristics/WeightComposit.h"
#include "Solver/AStar.h"
#include "Solver/IDDFS.h"
#include "Solver/BestFirst.h"

int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest4.txt");
	std::ofstream ofs("Answer.txt");
	Solver* solver;
	WeightComposit* density;

	density = new WeightComposit();
	
	density->AddHeuristic(new DensityAround(),1.0f);
	density->AddHeuristic(new Cavity(),-12.0f);
	solver = new BestFirst(prob,density);

	Answer ans = solver->Solve();
	ofs << ans << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}


#include <iostream>
#include <fstream>
#include "../Structure/Problem.h"
#include "Heuristics/Score.h"
#include "Heuristics/Density.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/Cavity.h"
#include "Heuristics/WeightComposit.h"
#include "Solver/AStar.h"
#include "Solver/DLS.h"
#include "Solver/BestFirst.h"
#include "Solver/PerfectBackTrack.h"
#include "Solver/IterativeCover.h"
#include "Perfect/DPBlockSize.h"
#include "Perfect/BlockSize.h"
#include "Perfect/ParityCheck.h"
#include "Perfect/PerfectComposit.h"
#include "Perfect/CavityBlocks.h"
#include "Perfect/MinTriming.h"


int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest5.txt");
	std::ofstream ofs("Answer.txt");

	//PerfectBackTrack p(prob);
	PerfectComposit* p = new PerfectComposit();
	WeightComposit * h = new WeightComposit();
	//IterativeCover*  solver = new IterativeCover(prob,h);
	Solver*  solver = new PerfectBackTrack(prob,h,p);
	//Solver*  solver = new BestFirst(prob,h);

	h->AddHeuristic(new DensityAround(),1.0f);
	h->AddHeuristic(new Cavity(),-15.0f);
	//p->AddHeuristic(new DPBlockSize());
	p->AddHeuristic(new CavityBlocks());
	p->AddHeuristic(new MinTriming());
	p->AddHeuristic(new ParityCheck());

	//solver->SetPerfect(p);
	//solver->SetHeuristic(h);
	solver->Solve().Export("Answer.txt");

	delete solver;
	delete p;
	delete h;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

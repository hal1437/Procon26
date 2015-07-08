
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
#include "Perfect/DPBlockSize.h"
#include "Perfect/BlockSize.h"
#include "Perfect/ParityCheck.h"
#include "Perfect/PerfectComposit.h"
#include "Perfect/CavityBlocks.h"


int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest6.txt");
	std::ofstream ofs("Answer.txt");
	
	PerfectBackTrack p(prob);
	PerfectComposit* p_h = new PerfectComposit();
	WeightComposit * h   = new WeightComposit();

	h->AddHeuristic(new DensityAround(),1.0f);
	h->AddHeuristic(new Cavity(),-15.0f);
	//p_h->AddHeuristic(new BlockSize());
	p_h->AddHeuristic(new DPBlockSize());
	p_h->AddHeuristic(new CavityBlocks());
	//p_h->AddHeuristic(new ParityCheck());
	p.SetPerfect(p_h);
	p.SetHeuristic(h);
	p.Solve();

	delete p_h;
	delete h;

	
	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

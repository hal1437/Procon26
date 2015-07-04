
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

int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest9.txt");
	std::ofstream ofs("Answer.txt");
	
	PerfectBackTrack p(prob);
	
	p.AddPerfect(new BlockSize());
	p.AddPerfect(new DPBlockSize());
	p.AddPerfect(new ParityCheck());
	p.Solve();

	/*
	BlockSize bs;
	BlockLayer l;
	l.resize(prob.Count());
	for(int i=0;i<prob.Count();i++)l[i].matrix = prob.GetBlock(i);
	bs.Execution(prob.GetField(),l);
	*/
	/*
	Solver* solver;
	WeightComposit* density;

	density = new WeightComposit();
	
	density->AddHeuristic(new DensityAround(),1.0f);
	density->AddHeuristic(new Cavity(),-16.0f);
	solver = new DLS(prob,density,1);

	//prob.GetField().Projection(prob.GetBlock(0),Transform(Point(0,9),Constants::ANGLE180,false));
	Answer ans = solver->Solve();
	ofs << ans << std::endl;
	//std::cout  << ans << std::endl;
	*/
	std::cout << "-----End of program-----" << std::endl;
	return 0;
}

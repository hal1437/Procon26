
#include <iostream>
#include <fstream>
#include "../Structure/Problem.h"
#include "Heuristics/Score.h"
#include "Heuristics/Density.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/Cavity.h"
#include "Heuristics/WeightComposit.h"
#include "Heuristics/SD.h"
#include "Heuristics/Dent.h"
#include "Heuristics/Frame.h"
#include "Heuristics/MinArea.h"
#include "Heuristics/Random.h"
#include "Solver/AStar.h"
#include "Solver/DLS.h"
#include "Solver/BestFirst.h"
#include "Solver/BestBeam.h"
#include "Solver/BestBackTrack.h"
#include "Solver/PerfectBackTrack.h"
#include "Solver/IterativeCover.h"
#include "Solver/QuickBeam.h"
#include "Solver/DoubleLimit.h"
#include "Perfect/DPBlockSize.h"
#include "Perfect/BlockSize.h"
#include "Perfect/ParityCheck.h"
#include "Perfect/PerfectComposit.h"
#include "Perfect/CavityBlocks.h"
#include "Perfect/MinTriming.h"
#include "Perfect/PPPP.h"
#include "../Utility/BenchMark.hpp"
#include <string>

int main(){

	std::cout << "\x1b[2J";

	BenchMark<1>()([](){

	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/1.txt");
	std::ofstream ofs("Answer.txt");

	//PerfectBackTrack p(prob);
	PerfectComposit* p = new PerfectComposit();
	WeightComposit * h = new WeightComposit();

	//Solver* solver = new PerfectBackTrack(prob,h,p);
	//Solver*  solver = new IterativeCover(prob,h);
	//Solver*  solver = new BestBackTrack(prob,h);
	Solver* solver = new DoubleLimit(prob,h);

	h->AddHeuristic(new DensityAround(),1.0f);
	//h->AddHeuristic(new Cavity()       ,-100.0f);
	h->AddHeuristic(new MinArea()      ,-10.0f);
	h->AddHeuristic(new Frame()       , 0.5f);
	//h->AddHeuristic(new SD()           ,-5.0);
	//h->AddHeuristic(new Dent()         ,-20.0);
	h->AddHeuristic(new Random()       ,2.0);
	//h->AddHeuristic(new DPBlockSize(),)
	
	//p->AddHeuristic(new CavityBlocks());
	p->AddHeuristic(new DPBlockSize());
	//p->AddHeuristic(new MinTriming());
	//p->AddHeuristic(new ParityCheck());
	//p->AddHeuristic(new PPPP());

	//solver->SetPerfect(p);
	//solver->SetHeuristic(h);
	solver->Solve().Export("Answer.txt");

	//delete solver;
	//delete p;
	//delete h;
	std::cout << "-----End of program-----" << std::endl;
	});

//quest6:（出力なし　マルチスレッド）
// ===========BENCHMARK RESULT===========
//     [COUNT]            1 times
// [FULL TIME]         1566 msec
//  [PER TIME]         1566 msec/function 
// ======================================
//
//quest6:（出力あり　マルチスレッド）
//===========BENCHMARK RESULT===========
//     [COUNT]            1 times
// [FULL TIME]         1985 msec
//  [PER TIME]         1985 msec/function 
// ======================================

	return 0;
}

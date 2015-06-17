
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
/*
int main(){
	std::cout << "----Begin of program----" << std::endl;
	Problem prob("../Problem/quest6.txt");
	std::ofstream ofs("Answer.txt");
	Solver* solver;
	WeightComposit* density;
	auto f = Field(prob.GetBlock(0).GetReverse(false).Rotate(Constants::ANGLE0)).Move(Point(0,0));
	//auto t = std::hash<Field>()(f);


	density = new WeightComposit();
	
	density->AddHeuristic(new DensityAround(),1.0f);
	density->AddHeuristic(new Cavity(),-16.0f);
	solver = new BestFirst(prob,density);

	//prob.GetField().Projection(prob.GetBlock(0),Transform(Point(0,9),Constants::ANGLE180,false));
	Answer ans = solver->Solve();
	ofs << ans << std::endl;
	//std::cout  << ans << std::endl;

	std::cout << "-----End of program-----" << std::endl;
	return 0;
}
*/
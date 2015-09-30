
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/WeightComposit.h"
#include "Heuristics/Frame.h"
#include "Heuristics/MinArea.h"
#include "Heuristics/Random.h"
#include "Solver/DoubleLimit.h"
#include "../Utility/BenchMark.hpp"

int main(){

	//コンソール全消し
	std::cout << "\x1b[2J";
	std::cout << "----Begin of program----" << std::endl;

	//ベンチマーク関数オブジェクト実行
	BenchMark<1>()([](){
		Problem prob("../Problem/41.txt");

		WeightComposit * h = new WeightComposit();
		Solver* solver = new DoubleLimit(prob,h);

		//評価関数追加
		h->AddHeuristic(new DensityAround()    ,   1.0f);
		h->AddHeuristic(new AntiDensityAround(),   0.8f);
		h->AddHeuristic(new MinArea()          ,  -1.5f);
		h->AddHeuristic(new Frame()            ,   0.0f);
		h->AddHeuristic(new Random()           ,   15.0);

		solver->Solve().Export("Answer.txt");

		//解放
		delete solver;
		delete h;
	});

	std::cout << "-----End of program-----" << std::endl;

	return 0;
}

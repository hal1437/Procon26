
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/WeightComposit.h"
#include "Heuristics/Frame.h"
#include "Heuristics/MinArea.h"
#include "Heuristics/Random.h"
#include "Solver/DoubleLimit.h"
#include "../Utility/BenchMark.hpp"
#include "Network/ProblemIO.h"


int main(){

	//コンソール全消し
	std::cout << "\x1b[2J";
	std::cout << "----Begin of program----" << std::endl;


	//ベンチマーク関数オブジェクト実行
	BenchMark<1>()([](){
		//std::string token = "0edebbbd0258632e";
		std::string token = "0123456789abcdef";
		//std::cout << HostSolver("testform26.procon-online.net") << std::endl;
		//Problem prob = GetProblem("testform26.procon-online.net","quest1.txt",token);

		
		Problem prob("../Problem/quest4.txt");

		WeightComposit * h = new WeightComposit();
		Solver* solver = new DoubleLimit(prob,h);

		//評価関数追加
		h->AddHeuristic(new DensityAround()    ,   1.0f);
		h->AddHeuristic(new AntiDensityAround(),   0.7f);
		h->AddHeuristic(new MinArea()          ,  -1.4f);
		h->AddHeuristic(new Frame()            ,   0.0f);
		h->AddHeuristic(new Random()           ,   3.0f);

		
		Answer ans = solver->Solve();
		ans.Export("Answer.txt");

		//解放
		delete solver;
		delete h;
		SendAnswer("testform26.procon-online.net","Answer.txt",token);
		//std::cout << SendAnswer("testform26.procon-online.net",token,ans) << std::endl;;
	});

	std::cout << "-----End of program-----" << std::endl;

	return 0;
}

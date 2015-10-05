
#include "../Structure/Problem.h"
#include "Heuristics/DensityAround.h"
#include "Heuristics/WeightComposit.h"
#include "Heuristics/Frame.h"
#include "Heuristics/MinArea.h"
#include "Heuristics/Random.h"
#include "Solver/DoubleLimit.h"
#include "../Utility/BenchMark.hpp"
#include "Http/SendAnswer.h"
#include <boost/asio.hpp>


int main(){



	//コンソール全消し
	std::cout << "\x1b[2J";
	std::cout << "----Begin of program----" << std::endl;

	//ベンチマーク関数オブジェクト実行
	BenchMark<1>()([](){
		std::cout << HostSolver("testform26.procon-online.net") << std::endl;
		Problem prob = GetProblem("testform26.procon-online.net","0123456789abcdef",1);
		
		//Problem prob("../Problem/41.txt");

		WeightComposit * h = new WeightComposit();
		Solver* solver = new DoubleLimit(prob,h);

		//評価関数追加
		h->AddHeuristic(new DensityAround()    ,   1.0f);
		h->AddHeuristic(new AntiDensityAround(),   0.9f);
		h->AddHeuristic(new MinArea()          ,  -1.0f);
		h->AddHeuristic(new Frame()            ,   0.1f);
		h->AddHeuristic(new Random()           ,   15.0);

		Answer ans = solver->Solve();
		ans.Export("Answer.txt");

		//解放
		delete solver;
		delete h;
		//SendAnswer(HostSolver("testform26.procon-online.net"),"0123456789abcdef",1,"Host:testform26.procon-online.net");
		std::cout << SendAnswer("testform26.procon-online.net","0123456789abcdef",ans) << std::endl;;
	});

	std::cout << "-----End of program-----" << std::endl;

	return 0;
}

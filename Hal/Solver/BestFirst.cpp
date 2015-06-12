
#include "BestFirst.h"

BestFirst::BestFirst(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
BestFirst::~BestFirst(){
}
Answer BestFirst::Solve(){
	Answer ans(problem);
	Field field = problem.GetField();
	std::vector<Transform> hands;	
	const int count = problem.Count();
	
	std::cout << field << std::endl;
	//seach best first block
	ans.SetField(field);
	for(int i=0;i < count;i++){
		//std::cout << field << std::endl;
		hands = field.GetListLayPossible(problem.GetBlock(i));
		///std::cout << "p";

		std::cout << i << ":";
		if(hands.size() != 0){
			Transform best = *std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
				Field l_field,r_field;
				l_field = r_field = field;
				l_field.Projection(problem.GetBlock(count),lhs);
				r_field.Projection(problem.GetBlock(count),rhs);
				return heuristic->Execution(l_field) < heuristic->Execution(r_field);
			});
			std::cout << std::endl;
			std::cout << best.pos << std::endl;
			field.Projection(problem.GetBlock(count),best);
			std::cout << hands.size() << ":" << heuristic->Execution(field) << std::endl;
			std::cout << field << std::endl;
			ans.AddBlocks(best);
		}else{
			ans.AddBlocks();
			std::cout << "nothing" << std::endl;
		}
	}
	std::cout << field << std::endl;
	return ans;
}




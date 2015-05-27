
#include "BestFirst.h"

BestFirst::BestFirst(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
BestFirst::~BestFirst(){
}
Answer BestFirst::Solve(){
	Answer ans;
	Field field = problem.GetField();
	std::vector<Hand> hands;	
	const int count = problem.Count();
	
	std::cout << field << std::endl;
	//seach best first block
	ans.SetField(field);
	for(int i=0;i < count;i++){
		hands = field.GetListLayPossible(problem.GetBlock(i));
		if(hands.size() != 0){
			Hand best = *std::max_element(hands.begin(),hands.end(),[&](const Hand& lhs,const Hand& rhs){
				Field l_field,r_field;
				l_field = r_field = field;
				l_field.Projection(lhs);
				r_field.Projection(rhs);
				return heuristic->Execution(l_field) > heuristic->Execution(r_field);
			});
			std::cout << best.pos << std::endl;
			field.Projection(best.pos,(best.reverse ? best.block.GetReverse() : best.block).GetRotate(best.angle));
			std::cout << hands.size() << ":" << heuristic->Execution(field) << std::endl;
			std::cout << field << std::endl;
			ans.AddBlocks(best.block,best.pos,best.reverse,best.angle);
		}else{
			ans.AddBlocks();
			std::cout << "nothing" << std::endl;
		}
	}
	std::cout << field << std::endl;
	return ans;
}




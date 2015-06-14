
#include "BestFirst.h"
#include <random>

BestFirst::BestFirst(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
BestFirst::~BestFirst(){
}
Answer BestFirst::Solve(){
	Answer ans(problem);
	Field field = problem.GetField();
	Field block_field;
	std::vector<Transform> hands;	
	const int count = problem.Count();
	std::random_device rd;


	std::cout << field << std::endl;
	//seach best first block
	ans.SetField(field);


	//random transform
	
	while(1){
		Transform rand_trans(Point((rd() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
								   (rd() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
							 static_cast<Constants::ANGLE>((rd()%4)*90),
				  			 rd()%2);
		auto b = problem.GetBlock(0).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(rand_trans);
		std::cout << b << std::endl;
		if((field & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
			ans.AddBlocks(rand_trans);
			block_field.Projection(problem.GetBlock(0),rand_trans);
			break;
		}
	}
	/*
	Transform first(Point(0,0),Constants::ANGLE0,false);
	ans.AddBlocks(first);
	block_field.Projection(problem.GetBlock(0),first);
	*/
	std::cout << (field | block_field) << std::endl;
	for(int i=1;i < count;i++){
		//std::cout << block_field << std::endl;
		hands = block_field.GetListLayPossible(problem.GetBlock(i));

		hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& trans){
			return ((field & problem.GetBlock(i).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(trans)).count()>0);
		}),hands.end());

		std::cout << i << ":";
		if(hands.size() != 0){
			Transform best = *std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
				Field l_field,r_field;
				l_field = r_field = block_field;
				l_field.Projection(problem.GetBlock(i),lhs);
				r_field.Projection(problem.GetBlock(i),rhs);
				l_field |= field;
				r_field |= field;
				return heuristic->Execution(l_field) < heuristic->Execution(r_field);
			});
			std::cout << std::endl;
			std::cout << best << std::endl;
			
			auto b = problem.GetBlock(i).GetTransform<FIELD_WIDTH,FIELD_WIDTH>(best);
			
			block_field.Projection(b);
			std::cout << hands.size() << ":" << heuristic->Execution(field) << std::endl;
			std::cout << (field | block_field) << std::endl;
			ans.AddBlocks(best);
		}else{
			ans.AddBlocks();
			std::cout << "nothing" << std::endl;
		}
	}
	//std::cout << field << std::endl;
	return ans;
}





#include "IDDFS.h"

IDDFS::IDDFS(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h),
	DEPTH(1){
}
IDDFS::IDDFS(Problem prob,Heuristics* h,int depth):
	Solver(prob),
	heuristic(h),
	DEPTH(depth){
}
IDDFS::~IDDFS(){
}

std::vector<Hand> IDDFS::Iterative(Field field,int block_num,int depth){
	std::vector<Hand> hands = field.GetListLayPossible(problem.GetBlock(block_num));
	std::vector<std::vector<Hand>> answers;
	
	//std::cout << block_num << std::endl;
	//std::cout << problem.GetBlock(block_num) << std::endl;

	//パス
	hands.push_back(Hand());
	
	//展開
	//std::cout << hands.size() << " " << std::flush;
	for(Hand hand:hands){
		Field local_field = field;
		local_field.Projection(hand);
		if(depth > 1 && block_num < problem.Count()-1){
			//深化
			answers.push_back(Iterative(local_field,block_num+1,depth-1));
		}
	}
	//評価
	unsigned max_heuristics = 0;
	int max_index=0;
	for(int i=0;i<hands.size();i++){
		Field local_field = field;
		local_field.Projection(hands[i]);
		if(answers.size() != 0){
			for(Hand hand:answers[i]){
				local_field.Projection(hand);
			}
		}
		int heuristics = heuristic->Execution(local_field);
		std::cout << heuristics << std::endl;
		std::cout << local_field << std::endl;
		
		if(max_heuristics < heuristics){
			max_heuristics = heuristics;
			max_index = i;
		}
	}
	
	//回答生成
	std::vector<Hand> answer;
	answer.push_back(hands[max_index]);
	if(answers.size() != 0){
		for(Hand& hand:answers[max_index])answer.push_back(hand);
	}
	return answer;
} 

Answer IDDFS::Solve(){
	Answer ans;
	Field field = problem.GetField();
	std::vector<Hand> hands;	
	const int count = problem.Count();
	
	std::cout << field << std::endl;
	std::cout << problem.Count() << std::endl;
	//反復深化
	
	Hand hand = Hand(problem.GetBlock(0),Point(-1,-1),Constants::ANGLE::ANGLE0,false);
	field.Projection(hand);
	ans.AddBlocks(hand);

	for(int i=0;i<problem.Count()/DEPTH;i++){
		std::vector<Hand> hands = Iterative(field,(i*DEPTH)+1,DEPTH);
		for(Hand hand : hands){
			field.Projection(hand);
			ans.AddBlocks(hand);
			std::cout << field << std::endl;
		} 
	}

	ans.SetField(field);
	return ans;
}


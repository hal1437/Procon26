
#include "DLS.h"

DLS::DLS(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h),
	DEPTH(1){
}
DLS::DLS(Problem prob,Heuristics* h,int depth):
	Solver(prob),
	heuristic(h),
	DEPTH(depth){
}
DLS::~DLS(){
}

std::vector<Transform> DLS::Iterative(Field field,int block_num,int depth){
	std::vector<Transform> hands = field.GetListLayPossible(problem.GetBlock(block_num));
	std::vector<std::vector<Transform>> answers;
	
	//std::cout << block_num << std::endl;
	//std::cout << problem.GetBlock(block_num) << std::endl;

	//パス
	hands.push_back(Transform());
	
	//展開
	//std::cout << hands.size() << " " << std::flush;
	for(Transform hand:hands){
		Field local_field = field;
		local_field.Projection(problem.GetBlock(block_num),hand);
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
		local_field.Projection(problem.GetBlock(block_num),hands[i]);
		if(answers.size() != 0){
			for(int j=0;j<answers[i].size();j++){
				local_field.Projection(problem.GetBlock(block_num+j),answers[i][j]);
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
	std::vector<Transform> answer;
	answer.push_back(hands[max_index]);
	if(answers.size() != 0){
		for(Transform& hand:answers[max_index])answer.push_back(hand);
	}
	return answer;
} 

Answer DLS::Solve(){
	Answer ans(problem);
	Field field = problem.GetField();
	std::vector<Transform> hands;	
	const int count = problem.Count();
	
	std::cout << field << std::endl;
	std::cout << problem.Count() << std::endl;
	//反復深化
	
	Transform hand = Transform(Point(-1,-1),Constants::ANGLE::ANGLE0,false);
	field.Projection(problem.GetBlock(0),hand);
	ans.AddBlocks(hand);

	for(int i=0;i<problem.Count()/DEPTH;i++){
		std::vector<Transform> hands = Iterative(field,(i*DEPTH)+1,DEPTH);
		for(int j=0;j<hands.size();j++){
			field.Projection(problem.GetBlock(i*DEPTH+j), hands[i]);
			ans.AddBlocks(hand);
			std::cout << field << std::endl;
		} 
	}

	ans.SetField(field);
	return ans;
}


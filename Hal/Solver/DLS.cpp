
#include "DLS.h"
#include <random>
#include <set>

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
	hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& trans){
		return ((problem.GetField() & problem.GetBlock(block_num).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(trans)).count()>0);
	}),hands.end());

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
		//std::cout << heuristics << std::endl;
		//std::cout << local_field << std::endl;
		
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

	std::set<Field> ff;
	while(1){

	Answer ans(problem);
	Field field;
	std::vector<Transform> hands;	
	const int count = problem.Count();
	std::random_device rd;
	
	std::cout << field << std::endl;
	//std::cout << problem.Count() << std::endl;
	//反復深化
	
	while(1){
		Transform rand_trans(Point((rd() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
								   (rd() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
							 static_cast<Constants::ANGLE>((rd()%4)*90),
				  			 rd()%2);
		auto b = problem.GetBlock(0).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(rand_trans);
		
		bool f =false;
		for(int i=0;i<FIELD_HEIGHT;i++){
			for(int j=0;j<FIELD_WIDTH;j++){
				Point pos(rand_trans.pos + Point(i,j));
				if(b[i][j]){
					if(pos.x<0 || pos.x>FIELD_WIDTH ||
					   pos.y<0 || pos.y>FIELD_HEIGHT )f =true;
				}
			}
		}
		if(f)continue;
		
		if(ff.find(b) != ff.end())continue;
		//std::cout << b << std::endl;
		if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
			ff.insert(b);
			ans.SetTransform(0,rand_trans);
			field.Projection(problem.GetBlock(0),rand_trans);
			break;
		}
	}

	//Transform hand = Transform(Point(-1,-1),Constants::ANGLE::ANGLE0,false);
	//field.Projection(problem.GetBlock(0),hand);
	//ans.AddBlocks(hand);

	for(int i=0;i<problem.Count()/DEPTH;i++){
		std::vector<Transform> hands = Iterative(field,(i*DEPTH+1),DEPTH);
		for(int j=0;j<hands.size();j++){
			//std::cout << i*DEPTH+1 << std::endl;
			field.Projection(problem.GetBlock(i*DEPTH+j+1), hands[j]);
			ans.SetTransform(i*DEPTH+j+1,hands[j]);
			std::cout << field << std::endl;
		} 
	}

	//ans.SetField(field);
	//return ans;
	std::cout << ff.size() << std::endl;
	std::cout << (~(field|problem.GetField())).count() << std::endl;
	std::cout <<   (field|problem.GetField()) << std::endl;
	if((~field).count()<8)return ans;

	}
}


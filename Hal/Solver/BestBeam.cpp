
#include "BestBeam.h"
//#include "../../Utility/Timewait.hpp"



BestBeam::Factor::Factor(){
	heuristic = std::numeric_limits<double>::max();
}
BestBeam::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}
bool operator==(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.field == rhs.field);
}

bool operator<(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.heuristic > rhs.heuristic);
}

BestBeam::BestBeam(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
BestBeam::~BestBeam(){

}


Answer BestBeam::Solve(){

	std::vector<Factor> list;//探索リスト

	//初期手
	list.push_back(Factor());
	
	list[0].field = problem.GetField();
	std::cout << list.size() << std::endl;

	//探索ループ
	while(!list.empty()){
		const Factor top = list.front();
		list.erase(list.begin());

		//完了
		if(~(top.field).count() == 0)break;
		//継続
		if(log.find(top.field) != log.end())continue;
		else log.insert(top.field);
		std::cout << top.field << std::endl;
		//Timewait(1000);

		//不意にGetListLayPossible
		std::vector<Transform> hands = top.field.GetListLayPossible(problem.GetBlock(top.transes.size()),problem.GetField());
		
		//パスも追加
		hands.push_back(Transform());
		
		//とりあえず全部突っ込む
		for(Transform hand:hands){
			std::vector<Transform> tmp = top.transes;
			tmp.push_back(hand);
			

			Factor fact;
			fact.field     = top.field.GetProjection(problem.GetBlock(top.transes.size()),hand);
			fact.heuristic = heuristic->Execution(fact.field);
			fact.transes   = tmp;
			
			list.push_back(fact);
		}
		
		//トリム
		std::sort(list.begin(),list.end());
		list.erase(std::unique(list.begin(),list.end()),list.end());
		if(list.size() > BEAM_DEPTH)list.erase(list.begin() + BEAM_DEPTH,list.end());
	
		std::cout << list.size() << std::endl;
		/*
		for(int i=0;i<list.size() ;i++){
			std::cout << i << ":" << list[i].heuristic << std::endl;
		}*/
	}

	while(1){}
	Answer ans;
	return ans;
}



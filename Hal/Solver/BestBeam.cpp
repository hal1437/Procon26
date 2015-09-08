
#include "BestBeam.h"



BestBeam::Factor::Factor(){
	heuristic = std::numeric_limits<double>::max();
}
BestBeam::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}
bool operator==(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.field == rhs.field && lhs.transes.size() == rhs.transes.size());
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
	
	//list[0].field = problem.GetField();
	std::cout << list.size() << std::endl;

	//探索ループ
	while(!list.empty()){
		int loop_count = std::min(100,static_cast<int>(list.size()));
		for(int i = 0;i < loop_count;i++){
			const Factor top = list.front();
			//完了
			if((~(top.field | problem.GetField())).count() == 0)break;
			
			//pop
			list.erase(list.begin());

			//終端
			if(top.transes.size() == problem.Count())continue;

			//盤面出力
			std::cout << "ループ  ：" << i << std::endl;
			std::cout << "ビーム　：" << list.size() << "/" << BEAM_DEPTH << std::endl;
			std::cout << "空きマス：" << (~(top.field | problem.GetField())).count() << std::endl;
			std::cout << "深さ　　：" << top.transes.size() << std::endl;
			std::cout << "評価値　：" << top.heuristic << std::endl;
			std::cout << "盤面状態：\n" << (top.field | problem.GetField()) << std::endl;
			Timewait(100);

			//遷移
			std::vector<Transform> hands = top.field.GetListLayPossible(problem.GetBlock(top.transes.size()),problem.GetField(),top.transes.size()==0);
			//std::cout << problem.GetBlock(top.transes.size()) << std::endl;
			//std::cout << hands.size() << std::endl;
	
			//パスも追加
			hands.push_back(Transform());
		
			//キューに追加
			for(Transform hand:hands){
				std::vector<Transform> tmp = top.transes;
				tmp.push_back(hand);

				Factor fact;
				fact.field     = top.field.GetProjection(problem.GetBlock(top.transes.size()),hand);
				fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
				fact.transes   = tmp;
				
				//探索済みでなければ追加
				if(log.find(fact.field) == log.end()){
					list.push_back(fact);
					log.insert(fact.field);
				}
			
			}
		}
		
		//ソートし、ビーム幅でソート
		std::sort(list.begin(),list.end());
		list.erase(std::unique(list.begin(),list.end()),list.end());
		if(list.size() > BEAM_DEPTH)list.erase(list.begin() + BEAM_DEPTH,list.end());
	
		/*
		for(int i=0;i<list.size() ;i++){
			std::cout << i << ":" << list[i].heuristic << std::endl;
		}*/
	}

	//以下完了時に実行
	if(list.empty())std::cout << "ビーム切れ" << std::endl;
	else std::cout << "完了" << std::endl;
	std::cout << (~list.front().field).count() << std::endl;
	std::cout << list.front().field << std::endl;
	
	for(Transform trans : list.front().transes){
		std::cout << trans << std::endl;;
	}
	while(1){}
	Answer ans;
	return ans;
}



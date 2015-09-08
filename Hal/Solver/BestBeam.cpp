
#include "BestBeam.h"



BestBeam::Factor::Factor(){
	heuristic = std::numeric_limits<double>::min();
}
BestBeam::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}
bool operator==(const Transform& lhs ,const Transform& rhs){
	return (lhs.angle   == rhs.angle &&
			lhs.pos     == rhs.pos   &&
			lhs.reverse == rhs.reverse);
}


bool operator==(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.transes == rhs.transes);
}

bool operator<(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.heuristic < rhs.heuristic);
}
bool operator>(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
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
	std::set<Factor> log;	//探索記録
	Factor best;
	Answer ans(problem);

	//初期手
	list.push_back(Factor());
	
	//list[0].field = problem.GetField();
	std::cout << list.size() << std::endl;

	//探索ループ
	while(!list.empty()){
		int loop_count = std::min(BEAM_DEPTH,static_cast<int>(list.size()));
		for(int i = 0;i < loop_count;i++){
			const Factor top = list.front();
			
			
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
			std::cout << "盤面状態：\n" << problem.GetBlock(top.transes.size());
			std::cout << (top.field | problem.GetField()) << std::endl;
			
			//完了
			best = std::max(top,best);
			if((~(top.field | problem.GetField())).count() == 0){
				best = top;
				list.clear();
				break;
			}
			//Timewait(100);


			//遷移
			std::vector<Transform> hands = top.field.GetListLayPossible(problem.GetBlock(top.transes.size()),problem.GetField(),top.transes.size()==0);
			//std::cout << problem.GetBlock(top.transes.size()) << std::endl;
			//std::cout << hands.size() << std::endl;
	
			//パスも追加
			hands.push_back(Transform());
			

			//std::cout << "候補数　：" << hands.size() << std::endl;
			//キューに追加
			for(Transform hand:hands){
				std::vector<Transform> tmp = top.transes;
				tmp.push_back(hand);

				Factor fact;
				fact.field     = top.field.GetProjection(problem.GetBlock(top.transes.size()),hand);
				fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
				fact.transes   = tmp;
				
				//探索済みでなければ追加
				//if(log.find(fact) == log.end()){
					list.push_back(fact);
					log.insert(fact);
				//}
			
			}
		}
		
		//ソートし、ビーム幅でソート
		std::sort(list.begin(),list.end(),std::greater<>());
		list.erase(std::unique(list.begin(),list.end()),list.end());
		if(list.size() > BEAM_DEPTH)list.erase(list.begin() + BEAM_DEPTH,list.end());
	
		/*
		for(int i=0;i<list.size() ;i++){
			std::cout << i << ":" << list[i].heuristic << std::endl;
		}*/
	}

	//以下完了時に実行
	if((~(best.field | problem.GetField())).count() != 0)std::cout << "ビーム切れ" << std::endl;
	else std::cout << "完了" << std::endl;
	std::cout << "最終評価値：" << best.heuristic << std::endl;
	std::cout << "空きマス数：" << (~(best.field | problem.GetField())).count() << std::endl;
	std::cout << "使用石数　：" << (best.transes.size() - std::count(best.transes.begin(),best.transes.end(),Transform())) << std::endl;

	
	for(int i=0;i < best.transes.size();i++){
		//std::cout << best.transes[i] << std::endl;
		ans.SetTransform(i,best.transes[i]);
	}

	return ans;
}




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

	/*
	for(int i=0;i<4;i++){
		for(int j=0;j<2;j++){
			Transform trans(Point(-1,-2),static_cast<Constants::ANGLE>(90*i),j);
			std::cout << trans << std::endl;
			std::cout << problem.GetBlock(5).GetTransform(trans) << std::endl;
		}
	}*/

	while(1){}


	//探索ループ
	while(!list.empty()){
		int loop_count = std::min(BEAM_DEPTH,static_cast<int>(list.size()));
		for(int i = 0;i < loop_count;i++){
			const Factor top = list.front();
			const Block  next = problem.GetBlock(top.transes.size());

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
			std::cout << "盤面状態：\n" << next;
			std::cout << (top.field | problem.GetField()) << std::endl;
			
			//完了
			best = std::max(top,best);
			if((~(top.field | problem.GetField())).count() == 5){
				best = top;
				list.clear();
				break;
			}
			//Timewait(100);


			//遷移
			std::vector<Transform> hands = top.field.GetListLayPossible(next,problem.GetField(),top.transes.size()==0);
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
				fact.field     = top.field.GetProjection(next,hand);
				fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
				fact.transes   = tmp;
				
				//探索済みでなければ追加
				if(log.find(fact) == log.end()){
					list.push_back(fact);
					log.insert(fact);
				}
			
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

	//conflict
	Field test = problem.GetField();
	for(int i=0;i < best.transes.size();i++){
		if((test & Field().GetProjection(problem.GetBlock(i),best.transes[i])).count()>0){
			std::cout << "conflicted" << std::endl;
		}
		test.Projection(problem.GetBlock(i),best.transes[i]);
		std::cout << i << std::endl;
		std::cout << test << std::endl;
	}


	//以下完了時に実行
	std::cout << "-----------------------------" << std::endl;
	if((~(best.field | problem.GetField())).count() != 0)std::cout << "ビーム切れ" << std::endl;
	else std::cout << "完了" << std::endl;
	std::cout << "盤面状態　：\n" << (best.field | problem.GetField()) << std::endl;
	std::cout << "最終評価値：" << best.heuristic << std::endl;
	std::cout << "空きマス数：" << (~(best.field | problem.GetField())).count() << std::endl;
	std::cout << "使用石数　：" << (problem.Count() - std::count(best.transes.begin(),best.transes.end(),Transform())) << std::endl;

	
	for(int i=0;i < problem.Count();i++){
		if(i < best.transes.size()){
			std::cout << i << ":" << best.transes[i] << std::endl;
			ans.SetTransform(i,best.transes[i]);
		}else{
			std::cout << i << ":" << Transform() << std::endl;
			ans.SetTransform(i,Transform());
		}
	}

	return ans;
}



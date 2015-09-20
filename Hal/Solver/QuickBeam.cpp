
#include "QuickBeam.h"
#include <random>
#include <set>


QuickBeam::Factor::Factor(){
	heuristic = std::numeric_limits<double>::min();
}
QuickBeam::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}

bool operator==(const QuickBeam::Factor& lhs,const QuickBeam::Factor& rhs){
	return (lhs.transes == rhs.transes);
}

bool operator<(const QuickBeam::Factor& lhs,const QuickBeam::Factor& rhs){
	return (lhs.transes < rhs.transes);
}
bool QuickBeam::Factor::HeuristicCompare(const Factor& lhs,const Factor& rhs){
	return lhs.heuristic > rhs.heuristic;
}


QuickBeam::QuickBeam(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
QuickBeam::~QuickBeam(){
}
Answer QuickBeam::Solve(){

	Answer ans(problem);
	Field field = problem.GetField();
	Field block_field;
	std::vector<Transform> hands;
	const int count = problem.Count();
	std::random_device rd;
	std::mt19937 mt(rd());
	Factor best;

	std::cout << field << std::endl;
	
	Transform first_trans;

	//seach best first block
	ans.SetField(field);

	while(1){
		std::set<Factor> log;	//探索記録
		std::vector<Factor> list;
		list.clear();

		//first random transform
		while(1){
			Transform rand_trans(Point((mt() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
									   (mt() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
								static_cast<Constants::ANGLE>((mt()%4)*90),
								mt()%2);
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
			
			//std::cout << b << std::endl;
			if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
				ans.SetTransform(0,rand_trans);
				block_field.Projection(problem.GetBlock(0),rand_trans);
				
				Factor fact;
				fact.field     = Field().GetProjection(problem.GetBlock(0),rand_trans);
				fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
				fact.transes   = {rand_trans};

				first_trans = rand_trans;
				list.push_back(fact);
				break;

			}
		}



		std::cout << (field | block_field) << std::endl;

		while(!list.empty()){
			BEAM_DEPTH = /*((500 - (~(list.front().field | problem.GetField())).count())/10)*/  3;
			int count = std::min(BEAM_DEPTH,static_cast<int>(list.size()));
			for(int i=0;i<count;i++){
				//先頭取り出してアレ
				const Factor top = list.front();
				const Block&  next = problem.GetBlock(top.transes.size());
				
				//pop
				list.erase(list.begin());

				//終端
				if(top.transes.size() == problem.Count())continue;
				
				//出力
				//std::cout << "ループ  ：" << i << std::endl;
				std::cout << "ビーム　：" << list.size() << "/" << BEAM_DEPTH << std::endl;
				std::cout << "空きマス：" << (~(top.field | problem.GetField())).count() << std::endl;
				std::cout << "深さ　　：" << top.transes.size() << std::endl;
				std::cout << "評価値　：" << top.heuristic << std::endl;
				std::cout << "盤面状態：\n" << next;
				std::cout << (top.field | problem.GetField()) << std::endl;

				//完了
				best = std::min(top,best,Factor::HeuristicCompare);
				if((~(top.field | problem.GetField())).count() == 0){
					best = top;
					list.clear();
					break;
				}

				//遷移
				std::vector<Transform> hands = top.field.GetListLayPossible(next,problem.GetField(),top.transes.size()==0);

				//パスも追加
				hands.push_back(Transform());

				//キューに追加
				for(Transform hand:hands){
					std::vector<Transform> tmp = top.transes;
					tmp.push_back(hand);

					Factor fact;
					fact.field     = top.field.GetProjection(next,hand);
					fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
					fact.transes   = tmp;
					
					//探索済みでなければ追加
					if(log.find(fact) == log.end()/* && fact.isPerfect(problem)*/){
						list.push_back(fact);
						log.insert(fact);
					}
				}
			}
			//ソートし、ビーム幅でソート
			std::sort(list.begin(),list.end(),Factor::HeuristicCompare);
			list.erase(std::unique(list.begin(),list.end()),list.end());
			if(list.size() > BEAM_DEPTH)list.erase(list.begin() + BEAM_DEPTH,list.end());
		}

		//std::cout << field << std::endl;
		//std::cout << (~(field | block_field)).count() << std::endl;
		//std::cout << (best.field | block_field) << std::endl;
		//std::cout << first_trans << std::endl;
		for(int i=0;i < problem.Count();i++){
			if(i < best.transes.size()){
				std::cout << i << ":" << best.transes[i] << std::endl;
				ans.SetTransform(i,best.transes[i]);
			}else{
				std::cout << i << ":" << Transform() << std::endl;
				ans.SetTransform(i,Transform());
			}
		}
		if((~(problem.GetField() | best.field)).count()<=37)return ans;
	}
}


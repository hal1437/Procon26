
#include "BestBeam.h"

int BestBeam::BEAM_DEPTH;


std::vector<Field> DivisionSpaces(const Field& field){
	std::vector<Field> answer;
	Field c_field(field);

	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(c_field[i][j]==0){
				Field tmp_field;
				tmp_field = ~tmp_field;
				tmp_field[i][j] = 0;
				std::queue<Point> queue;
				queue.push(Point(j,i));
				while(queue.size() != 0){
					CLOCKWISE_FOR(clock){
						Point s_pos = queue.front() + clock;
						if(s_pos.x >= 0 && s_pos.x < FIELD_WIDTH &&
						   s_pos.y >= 0 && s_pos.y < FIELD_HEIGHT &&
						   c_field[s_pos.y][s_pos.x]==0){
							c_field  [s_pos.y][s_pos.x] = 1;
							tmp_field[s_pos.y][s_pos.x] = 0;
							queue.push(s_pos);
						}
					} 
					queue.pop();
				}
				answer.push_back(tmp_field);
			}
		}
	}

	return answer;
}

BestBeam::Factor::Factor(){
	heuristic = std::numeric_limits<double>::min();
}
BestBeam::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}

bool operator==(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.transes == rhs.transes);
}

bool operator<(const BestBeam::Factor& lhs,const BestBeam::Factor& rhs){
	return (lhs.transes < rhs.transes);
}

bool BestBeam::Factor::HeuristicCompare(const Factor& lhs,const Factor& rhs){
	return lhs.heuristic > rhs.heuristic;
}

bool BestBeam::Factor::isPerfect(const Problem& problem)const{
	
	static std::set<std::pair<int,std::set<int>>> log;

	if((~(field | problem.GetField())).count()<100){
		BestBeam::BEAM_DEPTH=300;
		//-DivisionSpace-
		std::vector<Field> div = DivisionSpaces(field | problem.GetField());

		/*
		for(const Field& ff:div){
			std::cout << ff << std::endl;
		}
		Timewait(100);
		*/

		//DBBLockSize
		std::set<int> dp_size;
		
		//load
		for(const std::pair<int,std::set<int>> v:log){
			if(v.first == transes.size())dp_size = v.second;
		}
		//if notfound log
		if(dp_size == std::set<int>()){
			for(int i=transes.size();i<problem.Count();i++){
				int current_size = problem.GetBlock(i).count();
				dp_size.insert(current_size);
				for(int v : dp_size){
					if(v + current_size < 100)dp_size.insert(v + current_size);
				}
			}
			//save
			log.insert(std::make_pair(transes.size(),dp_size));
		}

		//全てのdiv.sizeうち一つでもdp_size内に存在していなければ。
		if(std::any_of(div.begin(),div.end(),[&](const Field& f){
			return (dp_size.find((~f).count()) ==dp_size.end());
		})){
			return false;
		}
	}
	return true;
}


BestBeam::BestBeam(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
	BEAM_DEPTH=100;
}
BestBeam::~BestBeam(){

}


Answer BestBeam::Solve(){

	std::vector<Factor> list;//探索リスト
	std::set<Factor> log;	//探索記録
	Factor best;
	Answer ans(problem);

	//初期手
	//((22,-3),270,false)
	/*
	Factor fact;
	Transform t(Point(22,-3),Constants::ANGLE270,false);
	fact.field     = Field().GetProjection(problem.GetBlock(0),t);
	fact.heuristic = heuristic->Execution(fact.field | problem.GetField());
	fact.transes   = {t};
	*/
	//list.push_back(fact);
	list.push_back(Factor());
	
	//list[0].field = problem.GetField();
	std::cout << list.size() << std::endl;

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
			best = std::min(top,best,Factor::HeuristicCompare);
			if((~(top.field | problem.GetField())).count() == 0){
				best = top;
				list.clear();
				break;
			}


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
				fact.heuristic = heuristic->Execution(fact.field | problem.GetField(),problem);
				fact.transes   = tmp;
				
				//探索済みでなければ追加
				if(log.find(fact) == log.end() && fact.isPerfect(problem)){
					list.push_back(fact);
					log.insert(fact);
				}
			
			}
		}
		
		//ソートし、ビーム幅でソート
		std::sort(list.begin(),list.end(),Factor::HeuristicCompare);
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



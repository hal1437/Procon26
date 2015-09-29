
#include "DoubleLimit.h"



std::vector<Field> DoubleLimit::DivisionSpaces(const Field& field){
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


DoubleLimit::DoubleLimit(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
DoubleLimit::~DoubleLimit(){

}

DoubleLimit::Factor::Factor(){
	heuristic = std::numeric_limits<double>::min();
}
DoubleLimit::Factor::Factor(Field f,double h):
	field(f),
	heuristic(h){
}


bool operator==(const DoubleLimit::Factor& lhs,const DoubleLimit::Factor& rhs){
	return (lhs.field == rhs.field);
}

bool operator<(const DoubleLimit::Factor& lhs,const DoubleLimit::Factor& rhs){
	return (lhs.field < rhs.field);
}

bool DoubleLimit::Factor::HeuristicCompare(const Factor& lhs,const Factor& rhs){
	return lhs.heuristic > rhs.heuristic;
}

bool DoubleLimit::isPerfect(const Factor& f)const{
	std::vector<Field> div = DivisionSpaces(f.field | problem.GetField());
	std::set<int>used;
	
	for(Field field : div){
		if((~field).count()>5)continue;
		int i;
		for(i = f.transes.size();i < problem.Count();i++){
			if((~field).count() == problem.GetBlock(i).count() && used.find(i) == used.end()){
				used.insert(i);
				break;
			}
		}
		if(i == problem.Count())return false;
	}
	return true;
}


Answer DoubleLimit::Solve(){

	std::vector<Factor> list;//探索リスト
	std::set<Factor> log;	//探索記録
	Factor best;
	Answer ans(problem);

	std::mutex mtx;
	bool complated = false;

	//初期手
	list.push_back(Factor());
	
	//探索ループ
	while(!list.empty() && !complated){
		int loop_count = std::min(PRIORITY_DEPTH,static_cast<int>(list.size()));
		
		std::vector<std::thread> threads;
		
		for(int i = 0;i < loop_count;i++){
			threads.push_back(std::thread([&](){
				mtx.lock();
				if(complated==true){
					mtx.unlock();
					return;
				}
				const Factor top = list.front();
				const Block  next = problem.GetBlock(top.transes.size());
				PRIORITY_DEPTH = 10 + top.transes.size()/10;
				//pop
				list.erase(list.begin());
				mtx.unlock();

				//終端
				if(top.transes.size() == problem.Count())return;

				//盤面出力
				
				mtx.lock();
				std::cout << "\x1b[0;0H";
				std::cout << "ループ  ：" << i << "                 \n";
				std::cout << "ビーム　：" << list.size() << "/" << BEAM_DEPTH << "(" << PRIORITY_DEPTH << ")" << "                 \n";
				std::cout << "空きマス：" << (~(top.field | problem.GetField())).count() << "                 \n";
				std::cout << "深さ　　：" << top.transes.size() << "                 \n";
				std::cout << "評価値　：" << top.heuristic << "                 \n";
				std::cout << "スレッド：" << std::this_thread::get_id() << std::endl;
				std::cout << "盤面状態：\n" << next;
				std::cout << (top.field | problem.GetField()) ;
				mtx.unlock();

				
				//完了
				best = std::min(top,best,Factor::HeuristicCompare);
				if((~(top.field | problem.GetField())).count() == 0){
					mtx.lock();
					best = top;
					list.clear();
					complated = true;
					mtx.unlock();
					return;
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
					fact.heuristic = heuristic->Execution(fact.field | problem.GetField(),problem);
					fact.transes   = tmp;
					
					//探索済みでなければ追加
					if(log.find(fact) == log.end() && isPerfect(fact)){
						
						mtx.lock();
						list.push_back(fact);
						log.insert(fact);
						mtx.unlock();
					}
				}
			}));
		}
		for(auto& t:threads)t.join();
		
		//ソートし、ビーム幅で枝刈り
		std::sort(list.begin(),list.end(),Factor::HeuristicCompare);
		list.erase(std::unique(list.begin(),list.end()),list.end());
		if(list.size() > BEAM_DEPTH)list.erase(list.begin() + BEAM_DEPTH,list.end());
	}


	//conflict
	Field test = problem.GetField();
	for(int i=0;i < best.transes.size();i++){
		if((test & Field().GetProjection(problem.GetBlock(i),best.transes[i])).count()>0){
			std::cout << "conflicted" << std::endl;
		}
		test.Projection(problem.GetBlock(i),best.transes[i]);
		//std::cout << i << std::endl;
		//std::cout << test << std::endl;
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


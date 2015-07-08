
#include "PerfectBackTrack.h"
#include "../../Utility/CLOCKWISE_FOR.hpp"
#include <queue>
#include <random>

void PerfectBackTrack::SetHeuristic(Heuristics_type* h){
	heuristic = h;
}
void PerfectBackTrack::SetPerfect(Perfect_type* p){
	perfect = p;
}

std::vector<Field> PerfectBackTrack::DivisionSpaces(const Field& field)const{
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

	//for(Field f :answer)std::cout << f << std::endl;
	

	return answer;
}

PerfectBackTrack::Iterative_type PerfectBackTrack::Iterative(const Field& field,const Field& block_field,BlockLayer layer)const{
	//反復
	std::cout << (~(field | block_field)).count() << std::endl;
	std::cout << (field | block_field) << std::endl;

	//終了
	if((~(field | block_field)).count()==0){
		while(1);
		return Iterative_type(std::vector<Transform>(),true);	//無効手　：　終了
	}
	if(layer.empty()){
		return Iterative_type(std::vector<Transform>(),false);	//無効手　：　継続
	}
	//抽出
	Block block = layer.front().matrix;
	std::vector<Transform> hands = block_field.GetListLayPossible(block,field);
	

	std::vector<Field> div = DivisionSpaces(field | block_field);
	//不適解削除
	hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& trans){
		//非完全問題
		bool f=false;
		for(const Field& div_field : div){
			if(!perfect->Execution(div_field,layer)){
				f=true;
				break;
			}
		}
		return f;
	}),hands.end());
	layer.erase(layer.begin());
	//std::cout << hands.size() << std::endl;


	//パス追加
	hands.push_back(Transform());

	//評価値ソート
	std::sort(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
		Field l_field,r_field;
		l_field = r_field = block_field;
		l_field.Projection(block,lhs);
		r_field.Projection(block,rhs);
		l_field |= field;
		r_field |= field;
		return heuristic->Execution(l_field) > heuristic->Execution(r_field);
	});
	
	//展開
	for(Transform hand:hands){
		Iterative_type return_value = Iterative(field,block_field.GetProjection(block,hand),layer);
		
		if(return_value.second){
			//完全手
			std::vector<Transform> tmp = return_value.first;
			tmp.push_back(hand);
			return Iterative_type(tmp,true);
		}else{
			//継続
		}
	}
	//不適
	return Iterative_type(std::vector<Transform>(),false);	//無効手　：　継続
}
Answer PerfectBackTrack::Solve(){
	Answer ans;
	BlockLayer blay;
	
	blay.resize(problem.Count());
	for(int j=0;j < this->problem.Count();j++)blay[j].matrix = problem.GetBlock(j);
	std::cout << std::boolalpha;
	std::cout << "[First field]:" << perfect->Execution(problem.GetField(),blay) << std::endl;
	std::cout << std::noboolalpha << std::endl;	
	std::cout << problem.GetField() << std::endl;


	Field block_field;
	std::random_device rd;
	//初期手乱択
	while(1){
		Transform rand_trans(Point((rd() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
								   (rd() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
							 static_cast<Constants::ANGLE>((rd()%4)*90),
				  			 rd()%2);
		Field b = problem.GetBlock(0).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(rand_trans);
		
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
		
		if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
			//ans.SetTransform(0,rand_trans);
			block_field.Projection(problem.GetBlock(0),rand_trans);
			break;
		}
	}

	blay.erase(blay.begin());
	std::cout << (problem.GetField() | block_field) << std::endl;
	Iterative_type return_value = Iterative(problem.GetField(),block_field,blay);

	for(Transform& hand :return_value.first){
		std::cout << hand << std::endl;
	}
	
	return ans;
}

PerfectBackTrack::PerfectBackTrack(Problem prob):
	Solver(prob){
}
PerfectBackTrack::~PerfectBackTrack(){

}


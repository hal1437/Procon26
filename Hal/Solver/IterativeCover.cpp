
#include "IterativeCover.h"


Answer IterativeCover::Solve(){
	Answer answer(problem);
	
	//while(1){
	
	std::random_device rd;
	Field first = problem.GetField();
	Field block_field;
	BlockLayer layer;
	
	//ブロックコピー
	layer.resize(problem.Count());
	for(int i = 0;i < problem.Count();i++){
		layer[i].matrix = problem.GetBlock(i);
	}
	
	//初期位置
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
		
		//std::cout << b << std::endl;
		if((first & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
			layer[0].trans = rand_trans;
			block_field.Projection(problem.GetBlock(0),rand_trans);
			break;
		}
	}

	std::cout << "first map" << std::endl;
	std::cout << (first | block_field)  << std::endl;

	//BEGINING_NUMまでBestFirst
	for(int i=1;i<BEGINING_NUM;i++){
		std::vector<Transform> hands = block_field.GetListLayPossible(problem.GetBlock(i),first);
		if(hands.size()!=0){
			Transform best = *std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
				Field l_field,r_field;
				l_field = r_field = block_field;
				l_field.Projection(problem.GetBlock(i),lhs);
				r_field.Projection(problem.GetBlock(i),rhs);
				return heuristic->Execution(l_field) < heuristic->Execution(r_field);
			});
			
			auto b = problem.GetBlock(i).GetTransform<FIELD_WIDTH,FIELD_WIDTH>(best);
			
			block_field.Projection(b);
			layer[i].trans = best;
		}else{
			layer[i].trans = Transform();
			std::cout << "nothing" << std::endl;
		}
	}

	std::cout << "second map" << std::endl;
	std::cout << (first | block_field)  << std::endl;

	Field future_mask;
	
	//大きい順にとにかく置く
	for(int i = 16;i >= 0;i--){
		
		Field sequense_field = block_field;
		for(int j = BEGINING_NUM;j < problem.Count();j++){
			
			//std::cout << j << ":" << std::endl;
			
			//未決定、検索中サイズと一致
			if(problem.GetBlock(j).count() == i){
				
				//最善手の検索
				std::cout << j << ":" << std::endl;
				std::cout << sequense_field << std::endl;

				//予定地にぶつからない、かつ隣接する
				std::vector<Transform> hands = sequense_field.GetListLayPossible(layer[j].matrix,future_mask | first);
				
				//置ければ
				if(hands.size()!=0){
					
					//いいやつ
					Transform best = *std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
						Field l_field,r_field;
						l_field = r_field = block_field;
						l_field.Projection(problem.GetBlock(j),lhs);
						r_field.Projection(problem.GetBlock(j),rhs);
						return heuristic->Execution(l_field) < heuristic->Execution(r_field);
					});
					
					layer[j].trans = best;
					
					//置く予定地
					future_mask.Projection(layer[j].matrix,best);
					//置くとこ
					sequense_field.Projection(layer[j].matrix,best);

				}
			}else if(problem.GetBlock(j).count() > i){
				//でかい

				//決定済み
				sequense_field.Projection(layer[j].matrix,layer[j].trans);
			}

			/*
			if(decision[j]==true){
				//マスク追加
				placed_mask.Projection(layer[j].matrix,layer[j].trans);
			}*/
			//for(int c=0;c<1000000;c++);
		}
	}
	
	
	bool exit_flag = false;
	//Field field = problem.F;
	//回答出力
	for(int i=0;i < problem.Count();i++){
		std::cout << i << ":" << layer[i].trans << std::endl;
		answer.SetTransform(i,layer[i].trans);
	}

	//}

	return answer;
}

IterativeCover::IterativeCover(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){

}
IterativeCover::~IterativeCover(){

}


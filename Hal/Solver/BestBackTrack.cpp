
#include "BestBackTrack.h"
#include <random>
#include <set>



BestBackTrack::BestBackTrack(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
	//初期化
	for(int i=0;i<FIELD_HEIGHT ;i++){
		for(int j=0;j<FIELD_HEIGHT ;j++){
			step_field[i][j] = -1;
		}
	}
	transes.resize(prob.Count());
}
BestBackTrack::~BestBackTrack(){
}


void BestBackTrack::ProjectionStepfield(const Block& block,const Transform& trans,int depth){
	Field tmp;
	tmp.Projection(block,trans);
	
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(tmp[i][j]==true){
				step_field[i][j] = depth;
			}
		}
	}
}

int BestBackTrack::Iterative(Field field,int depth){

	//最初はランダム手ってことで
	if(depth == 0){
		while(1){
			//全部解くまで頑張って
			std::set<Transform> past;
			std::random_device rd;

			//い　つ　も　の
			Transform rand_trans;
			while(1){
				rand_trans = Transform(Point((rd() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
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
				if(past.find(rand_trans) != past.end())continue;
				past.insert(rand_trans);
	
				std::cout << b << std::endl;
				if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
					//ans.SetTransform(0,rand_trans);
					field.Projection(problem.GetBlock(0),rand_trans);
					break;
				}
			}
			//完了
			if(Iterative(field,depth+1) == -1)return -1;
		}
	}else if(depth == problem.Count() - 1){
		//最終手
		
		//欠番判定
		int lack = 256;
		for(int i=0;i<FIELD_HEIGHT;i++){
			for(int j=0;j<FIELD_WIDTH;j++){
				//欠番
				if(field[i][j] == false){
					//最も浅い隣接操作までバックトラック
					CLOCKWISE_FOR(clock){
						if(i+clock.y >= 0 && i+clock.y < FIELD_WIDTH &&
						   j+clock.x >= 0 && j+clock.x < FIELD_WIDTH){
							if(step_field[i+clock.y][j+clock.x] != -1){
								lack = std::min(lack,step_field[i+clock.y][j+clock.x]);
							}
						}
					}
				}
			}
		}
		if(lack==256)lack=-1;
		return lack;
	}else{
		//あとはだいたいBestFirst
		auto hands = field.GetListLayPossible(problem.GetBlock(depth),problem.GetField());

		std::cout << depth << ":" << std::endl;
		for(int i=0;i<hands.size();i++){
			Transform best = *std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
				Field l_field,r_field;
				l_field = r_field = (field | problem.GetField());
				l_field.Projection(problem.GetBlock(depth),lhs);
				r_field.Projection(problem.GetBlock(depth),rhs);
				return heuristic->Execution(l_field) < heuristic->Execution(r_field);
			});
			
			Timewait(500);
			
			//反復
			ProjectionStepfield(problem.GetBlock(depth),best,depth);
			field.Projection(problem.GetBlock(depth),best);
			
			int it = Iterative(field,depth + 1);
			if(it < depth)return it;
			
			
			//復元
			ProjectionStepfield(problem.GetBlock(depth),best,-1);
			field.ReverseProjection(problem.GetBlock(depth),best);
		}

		//パス手
		return Iterative(field,depth + 1);

	}
		
	//}while(Iterative(field,depth+1) == depth);

	return -1;
}

Answer BestBackTrack::Solve(){
	Answer ans;
	Iterative(Field(),0);
}




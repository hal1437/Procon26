
#include "PerfectBackTrack.h"
#include "../../Utility/CLOCKWISE_FOR.hpp"
#include <queue>

void PerfectBackTrack::AddHeuristic(Heuristics_type* h){
	heuristic.push_back(h);
}
void PerfectBackTrack::AddPerfect(Perfect_type* p){
	perfect.push_back(p);
}

std::vector<Field> PerfectBackTrack::DivisionSpaces(const Field& field)const{
	std::vector<Field> answer;
	Field c_field(field);

	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(c_field[i][j]){
				Field tmp_field;
				tmp_field = ~tmp_field;
				std::queue<Point> queue;

				queue.push(Point(i,j));
				while(queue.size() != 0){
					CLOCKWISE_FOR(clock){
						Point s_pos = Point(i,j) + clock;
						if(s_pos.x >= 0 && s_pos.x < FIELD_WIDTH &&
						   s_pos.y >= 0 && s_pos.y < FIELD_HEIGHT){
							c_field[s_pos.y][s_pos.x] = 1;
							queue.push(s_pos);
						}
					} 
				}
				answer.push_back(tmp_field);
			}
		}
	}

	for(Field f :answer)std::cout << f << std::endl;
	

	return answer;
}

Answer PerfectBackTrack::Solve(){
	Answer ans;
	BlockLayer blay;
	
	blay.resize(problem.Count());
	for(int j=0;j < this->problem.Count();j++)blay[j].matrix = problem.GetBlock(j);
	std::cout << std::boolalpha;
	std::cout << "  BlockSize:" << perfect[0]->Execution(problem.GetField(),blay) << std::endl;
	std::cout << "DPBlockSize:" << perfect[1]->Execution(problem.GetField(),blay) << std::endl;
	std::cout << "ParityCheck:" << perfect[2]->Execution(problem.GetField(),blay) << std::endl;
	
	std::cout << std::noboolalpha;
	
	return ans;
}

PerfectBackTrack::PerfectBackTrack(Problem prob):
	Solver(prob){
}
PerfectBackTrack::~PerfectBackTrack(){

}


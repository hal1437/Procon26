
#include "WeightComposit.h"

void WeightComposit::AddHeuristic(h_type* heuristics,float weight){
	h_list.insert(std::pair<h_type*,float>(heuristics,weight));
}

unsigned WeightComposit::Execution(const Field& field){
	unsigned score = 0;
	for(std::pair<h_type*,float> h : h_list){
		score += h.first->Execution(field) * h.second;
	}
	return score;
}

WeightComposit::WeightComposit(){
}


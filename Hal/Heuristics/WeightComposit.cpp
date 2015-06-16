
#include "WeightComposit.h"

void WeightComposit::AddHeuristic(h_type* heuristics,double weight){
	h_list.insert(std::pair<h_type*,double>(heuristics,weight));
}

double WeightComposit::Execution(const Field& field){
	double score = 0;
	for(std::pair<h_type*,double> h : h_list){
		score += h.first->Execution(field) * h.second;
	}
	return score;
}

WeightComposit::WeightComposit(){
}


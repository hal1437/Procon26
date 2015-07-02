
#include "Composit.h"

void Composidddt::AddHeuristic(h_type* heuristics){
	h_list.insert(heuristics);
}

double Composit::Execution(const Field& field){
	double score = 0;
	for(std::pair<h_type*,double> h : h_list){
		if(h->Execution(field))return false;
	}
	return true;
}

Composit::Composit(){
}


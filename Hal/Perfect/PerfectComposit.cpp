
#include "PerfectComposit.h"

void PerfectComposit::AddHeuristic(h_type* heuristics){
	heuristicses.push_back(heuristics);
}

bool PerfectComposit::Execution(const Field& field,const BlockLayer& layer){
	double score = 0;
	for(h_type* h : heuristicses){
		if(!h->Execution(field,layer))return false;
	}
	return true;
}

PerfectComposit::PerfectComposit(){
}


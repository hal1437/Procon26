
#include "AStar.h"

AStar::AStar(Problem prob,Heuristics* h):
	Solver(prob),
	heuristic(h){
}
AStar::~AStar(){
}

Answer AStar::Solve(){
	Answer ans(problem);

	//f (n)= g (n) + h (n)

	return ans;
}



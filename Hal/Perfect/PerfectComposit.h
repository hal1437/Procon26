
#pragma once
#include "../Structures.h"
#include <vector>

class PerfectComposit : public Perfect<Field,BlockLayer>{
private:
	typedef Perfect<Field,BlockLayer> h_type;
	std::vector<h_type*> heuristicses;

public:

	void AddHeuristic(h_type* heuristics);

	bool Execution(const Field& field,const BlockLayer& layer);
	
	PerfectComposit();
};


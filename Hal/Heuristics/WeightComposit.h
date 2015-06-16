
#pragma once
#include "../Structures.h"
#include <map>

class WeightComposit : public Heuristics<unsigned,Field>{
private:
	typedef Heuristics<unsigned,Field> h_type;
	std::map<h_type*,float> h_list;

public:

	void AddHeuristic(h_type* heuristics,float weight);

	unsigned Execution(const Field& field);

	WeightComposit();
};


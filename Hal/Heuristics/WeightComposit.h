
#pragma once
#include "../Structures.h"
#include <map>

class WeightComposit : public Heuristics<double,Field>{
private:
	typedef Heuristics<double,Field> h_type;
	std::map<h_type*,double> h_list;

public:

	void AddHeuristic(h_type* heuristics,double weight);

	double Execution(const Field& field);

	WeightComposit();
};


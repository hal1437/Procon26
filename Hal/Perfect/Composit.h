
#pragma once
#include "../Structures.h"
#include <vector>

class Composit : public Perfect<Field,Layer>{
private:
	typedef Perfect<Field,Field> h_type;
	std::vector<h_type> h_type*;

public:

	void AddHeuristic(h_type* heuristics);

	bool Execution(const Field& field,const Layer& layer);
	
	Composit();
};


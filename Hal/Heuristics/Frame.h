
#pragma once
#include "../Structures.h"

class Frame : public Heuristics<double,Field,Problem>{
public:
	double Execution(const Field& field,const Problem& prob);

	Frame();
};


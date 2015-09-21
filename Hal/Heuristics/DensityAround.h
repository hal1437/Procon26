
#pragma once
#include "../Structures.h"
#include "../../Structure/Problem.h"

class DensityAround : public Heuristics<double,Field,Problem>{
public:
	double Execution(const Field& field,const Problem& prob);

	DensityAround();
};


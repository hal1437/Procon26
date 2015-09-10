
#pragma once
#include "../Structures.h"

class DensityAround : public Heuristics<double,Field>{
public:
	double Execution(const Field& field);

	DensityAround();
};


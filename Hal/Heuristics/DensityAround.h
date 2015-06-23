
#pragma once
#include "../Structures.h"

class DensityAround : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	DensityAround();
};



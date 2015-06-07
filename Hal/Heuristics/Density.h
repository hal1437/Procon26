
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"

class Density : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	Density();
};


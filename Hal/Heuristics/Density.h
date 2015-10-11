
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"

class Density : public Heuristics<double,Field>{
public:
	double Execution(const Field& field);

	Density();
};


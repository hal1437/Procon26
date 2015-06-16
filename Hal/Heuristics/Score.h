
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"

class Score : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	Score();
};


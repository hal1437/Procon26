
#pragma once
#include "../../Structure/Field.h"
#include "../../Structure/Heuristic.h"

class Score : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	Score();
};


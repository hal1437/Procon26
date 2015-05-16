
#pragma once
#include "../../Structure/Field.h"
#include "../../Structure/Heuristic.h"


class Density : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	Density();
};


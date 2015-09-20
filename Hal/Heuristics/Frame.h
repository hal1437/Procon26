
#pragma once
#include "../Structures.h"

class Frame : public Heuristics<double,Field>{
public:
	double Execution(const Field& field);

	Frame();
};



#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"
#include <set>

class Near : public Heuristics<double,Field,Problem>{
private:
	int FillAround(Field& field,const Point& pos)const;

public:

	const static int THRESHOLD=16;
	double Execution(const Field& field,const Problem& prob);

	Near();
};



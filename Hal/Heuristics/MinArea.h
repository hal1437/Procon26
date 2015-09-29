
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"
#include <set>

class MinArea : public Heuristics<double,Field,Problem>{
private:
	int FillAround(Field& field,const Point& pos)const;

public:

	const static int THRESHOLD=25;
	double Execution(const Field& field,const Problem& prob);

	MinArea();
};


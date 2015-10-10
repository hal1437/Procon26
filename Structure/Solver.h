
#pragma once 
#include "Problem.h"
#include "Answer.h"


class Solver{
protected:
	Problem problem;
public:
	virtual Answer Solve() = 0;
	
	explicit Solver(Problem prob):problem(prob){};
};


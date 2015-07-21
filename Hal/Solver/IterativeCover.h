
#pragma once
#include "../Structures.h"
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

class IterativeCover:public Solver{
private:
	typedef Heuristics<double,Field> Heuristics;

	static constexpr int BEGINING_NUM = 4;
	static constexpr int DIVISION_SIZE_NUM = 3;
	static constexpr int DIVISION_SIZES[DIVISION_SIZE_NUM] = {10,4,1};

private:

	Heuristics* heuristic;

public:

	Answer Solve()override;

	IterativeCover(Problem prob,Heuristics* h);
	virtual ~IterativeCover();
	
};

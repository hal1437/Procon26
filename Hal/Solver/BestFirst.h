
#pragma once 
#include "../Structures.h"
#include <vector>
#include <algorithm>

//最良優先探索
class BestFirst:public Solver{
	typedef Heuristics<double,Field> Heuristics;
public:
	Heuristics* heuristic;

public:

	Answer Solve()override;

	BestFirst(Problem prob,Heuristics* h);
	virtual ~BestFirst();
};


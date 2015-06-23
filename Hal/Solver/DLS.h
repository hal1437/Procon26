#pragma once 
#include "../Structures.h"
#include <vector>
#include <algorithm>

//反復深化
class DLS:public Solver{
	typedef Heuristics<unsigned,Field> Heuristics;
public:
	Heuristics* heuristic;
	const int DEPTH;
public:

	std::vector<Transform> Iterative(Field field,int block_num,int depth);

	Answer Solve()override;

	DLS(Problem prob,Heuristics* h);
	DLS(Problem prob,Heuristics* h,int depth);
	virtual ~DLS();
};


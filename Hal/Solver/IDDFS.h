#pragma once 
#include "../Structures.h"
#include <vector>
#include <algorithm>

//反復深化
class IDDFS:public Solver{
	typedef Heuristics<unsigned,Field> Heuristics;
public:
	Heuristics* heuristic;
	const int DEPTH;
public:

	std::vector<Transform> Iterative(Field field,int block_num,int depth);

	Answer Solve()override;

	IDDFS(Problem prob,Heuristics* h);
	IDDFS(Problem prob,Heuristics* h,int depth);
	virtual ~IDDFS();
};


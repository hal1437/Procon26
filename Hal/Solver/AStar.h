
#pragma once
#include "../Structures.h"

//A*アルゴリズム
class AStar :public Solver{
	typedef Heuristics<unsigned,Field> Heuristics;
protected:
	Heuristics* heuristic;

public:

	Answer Solve()override;

	AStar(Problem prob,Heuristics* h);
	virtual ~AStar();
};



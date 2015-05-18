
#pragma once 
#include "../Structures.h"
#include <vector>

//最良優先探索
class BestFirst:public Solver{
	typedef Heuristics<unsigned,Field> Heuristics;
public:
	Heuristics* heuristic;

public:
	static std::vector<Point> PossibleLayList(Block block);

	Answer Solve()override;

	BestFirst(Problem prob,Heuristics* h);
	virtual ~BestFirst();
};


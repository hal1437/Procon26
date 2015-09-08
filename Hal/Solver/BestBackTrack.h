
#pragma once 
#include "../Structures.h"
#include <vector>
#include <set>
#include <algorithm>
#include "../../Utility/TimeWait.h"

//いい感じにバックトラック
class BestBackTrack:public Solver{
	typedef Heuristics<double,Field>   Heuristics;

public:
	Heuristics* heuristic;

	int step_field[FIELD_WIDTH][FIELD_HEIGHT];
	std::vector<Transform> transes;

private:
	void ProjectionStepfield(const Block& block,const Transform& trans,int depth);

	int Iterative(Field field,int depth);

public:

	Answer Solve()override;

	BestBackTrack(Problem prob,Heuristics* h);
	virtual ~BestBackTrack();
};



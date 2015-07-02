
#pragma once 
#include "../Structures.h"
#include <vector>
#include <algorithm>

//完全回答
class PerfectBackTrack:public Solver{
	typedef Heuristics<double,Field>   Heuristics_type;
	typedef Perfect<Field ,BlockLayer> Perfect_type;
public:
	std::vector<Heuristics_type*> heuristic;
	std::vector<Perfect_type*>    perfect;

	void AddHeuristic(Heuristics_type* h);
	void AddPerfect  (Perfect_type* p);
	std::vector<Field> DivisionSpaces(const Field& field)const;

public:

	Answer Solve()override;

	PerfectBackTrack(Problem prob);
	virtual ~PerfectBackTrack();
};


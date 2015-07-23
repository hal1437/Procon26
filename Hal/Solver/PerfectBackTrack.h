
#pragma once 
#include "../Structures.h"
#include <vector>
#include <set>
#include <algorithm>

//完全回答
class PerfectBackTrack:public Solver{
	typedef Heuristics<double,Field>   Heuristics_type;
	typedef Perfect<Field ,BlockLayer> Perfect_type;
	typedef std::pair<std::vector<Transform>,bool> Iterative_type;
public:
	Heuristics_type* heuristic;
	Perfect_type*    perfect;
	std::set<std::pair<Block,Transform>> past_trans;

private:
	std::vector<Field> DivisionSpaces(const Field& field)const;
	Iterative_type Iterative(const Field& field,const Field& block_field,BlockLayer layer);

public:
	void SetHeuristic(Heuristics_type* h);
	void SetPerfect  (Perfect_type* p);

	Answer Solve()override;

	PerfectBackTrack(Problem prob,Heuristics_type* h,Perfect_type* p);
	virtual ~PerfectBackTrack();
};


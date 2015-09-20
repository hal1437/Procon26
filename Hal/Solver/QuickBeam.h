
#pragma once 
#include "../Structures.h"
#include "../../Utility/Timewait.h"
#include <vector>
#include <algorithm>

//最良優先ビーム
class QuickBeam:public Solver{
	typedef Heuristics<double,Field> Heuristics;
public:
	int BEAM_DEPTH = 3;
	Heuristics* heuristic;

	struct Factor{
		Field field;
		double heuristic = std::numeric_limits<double>::min();
		std::vector<Transform> transes;

		static bool HeuristicCompare(const Factor& lhs,const Factor& rhs);
		
		bool isPerfect(const Problem& problem)const;
		Factor();
		Factor(Field f,double h);

	};

public:

	Answer Solve()override;

	QuickBeam(Problem prob,Heuristics* h);
	virtual ~QuickBeam();
};



#pragma once 
#include "../Structures.h"
#include "../../Utility/Timewait.h"
#include <vector>
#include <queue>
#include <functional>
#include <set>
#include <algorithm>
#include <limits>

//二重制限探索
class DoubleLimit:public Solver{
	typedef Heuristics<double,Field,Problem> Heuristics;
	typedef Perfect<Field ,BlockLayer> Perfect;
	int BEAM_DEPTH     = 1000;//ビーム幅
	int PRIORITY_DEPTH = 50;//ビーム幅
	//PRIORITY_DEPTH <= BEAM_DEPTH

public:
	//要素
	struct Factor{
		Field field;
		double heuristic = std::numeric_limits<double>::min();
		std::vector<Transform> transes;

		static bool HeuristicCompare(const Factor& lhs,const Factor& rhs);
		
		//bool isPerfect(const Problem& problem)const;
		Factor();
		Factor(Field f,double h);

	};
public:
	Heuristics* heuristic;
public:


	static std::vector<Field> DivisionSpaces(const Field& field);

	bool isPerfect(const Factor& f)const;

	Answer Solve()override;
	

	DoubleLimit(Problem prob,Heuristics* h);
	virtual ~DoubleLimit();
};


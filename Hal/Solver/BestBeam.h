
#pragma once 
#include "../Structures.h"
#include "../../Utility/Timewait.h"
#include <vector>
#include <set>
#include <algorithm>
#include <limits>

//最良優先ビームサーチ
class BestBeam:public Solver{
	typedef Heuristics<double,Field> Heuristics;
	const static int BEAM_DEPTH = 100;//ビーム幅
	
public:
	//要素
	struct Factor{
		Field field;
		double heuristic = std::numeric_limits<double>::max();
		std::vector<Transform> transes;

		Factor();
		Factor(Field f,double h);
	};
public:
	Heuristics* heuristic;
	std::set<Field> log;
public:

	Answer Solve()override;

	BestBeam(Problem prob,Heuristics* h);
	virtual ~BestBeam();
};


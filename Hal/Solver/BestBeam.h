
#pragma once 
#include "../Structures.h"
#include "../../Utility/Timewait.h"
#include <vector>
#include <queue>
#include <functional>
#include <set>
#include <algorithm>
#include <limits>

std::vector<Field> DivisionSpaces(const Field& field);

//最良優先ビームサーチ
class BestBeam:public Solver{
	/*
	 *記録
	 *評価関数：密度　　 2.0f
	 *　　　　　閉空間数 -20.0f
	 *ビーム幅 30~200
	 *
	 * */

	typedef Heuristics<double,Field,Problem> Heuristics;
	typedef Perfect<Field ,BlockLayer> Perfect;
	static int BEAM_DEPTH;//ビーム幅
	
public:
	//要素
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
	Heuristics* heuristic;
public:

	Answer Solve()override;
	

	BestBeam(Problem prob,Heuristics* h);
	virtual ~BestBeam();
};


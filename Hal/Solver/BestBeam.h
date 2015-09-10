
#pragma once 
#include "../Structures.h"
#include "../../Utility/Timewait.h"
#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <limits>

//最良優先ビームサーチ
class BestBeam:public Solver{
	/*
	 *記録
	 *評価関数：密度　　 2.0f
	 *　　　　　閉空間数 -20.0f
	 *ビーム幅 30~200
	 *
	 * */

	typedef Heuristics<double,Field> Heuristics;
	const int BEAM_DEPTH = 100;//ビーム幅
	
public:
	//要素
	struct Factor{
		Field field;
		double heuristic = std::numeric_limits<double>::min();
		std::vector<Transform> transes;

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


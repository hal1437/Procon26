
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"

//減点評価
//へこむとつらいよね
class Dent : public Heuristics<double,Field,Problem>{
public:
	double Execution(const Field& field,const Problem& prob);

	Dent();
};



#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"

//減点評価
//へこむとつらいよね
class Dent : public Heuristics<double,Field>{
public:
	double Execution(const Field& field);

	Dent();
};


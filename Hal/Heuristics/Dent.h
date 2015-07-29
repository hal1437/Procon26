
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"

//減点評価
//へこむとつらいよね
class Dent : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	Dent();
};


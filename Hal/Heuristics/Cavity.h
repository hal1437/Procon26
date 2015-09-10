
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Utility/CLOCKWISE_FOR.hpp"

class Cavity : public Heuristics<double,Field>{
private:
	void FillAround(Field& field,const Point& pos)const;

public:
	double Execution(const Field& field);

	Cavity();
};


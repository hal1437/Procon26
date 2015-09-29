
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"
#include "../../Utility/CLOCKWISE_FOR.hpp"

class Cavity : public Heuristics<double,Field,Problem>{
private:
	void FillAround(Field& field,const Point& pos)const;

public:
	double Execution(const Field& field,const Problem& prob);

	Cavity();
};


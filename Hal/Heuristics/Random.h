
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"
#include <random>

class Random : public Heuristics<double,Field,Problem>{
private:
public:
	double Execution(const Field& field,const Problem& prob);

};


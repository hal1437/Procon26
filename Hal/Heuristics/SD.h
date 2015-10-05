
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"
#include "../../Structure/Problem.h"


//標準偏差
class SD : public Heuristics<double,Field,Problem>{
public:
	struct Point_f{
		float x,y;
		Point_f():x(0.0f),y(0.0f){}
		Point_f(float _x,float _y):x(_x),y(_y){}
	};
public:

	//集まってる度算出
	double Execution(const Field& field,const Problem& prob);

	SD();
};


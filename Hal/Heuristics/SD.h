
#pragma once
#include "../../Structure/Matrix.hpp"
#include "../../Structure/Heuristic.h"


//標準偏差
class SD : public Heuristics<double,Field>{
public:
	struct Point_f{
		float x,y;
		Point_f():x(0.0),y(0.0){}
		Point_f(float _x,float _y):x(_x),y(_y){}
	};
public:

	//集まってる度算出
	double Execution(const Field& field);

	SD();
};


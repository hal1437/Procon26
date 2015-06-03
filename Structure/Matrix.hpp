
#pragma once
#include "MultiBit.hpp"
#include "Point.h"
#include "Hand.h"
#include "../Utility/Constants.hpp"

template<size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class Matrix:private MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>{
private:
	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> Base;


public:
	

	virtual Matrix& Projection(const Hand& hand);
	virtual Matrix& Projection(const Matrix& mat,const Point& pos,Constants::ANGLE angle,bool reverse);

	Matrix() = default;
	Matrix(const Matrix& mat)  = default;
	Matrix(const Base&   base):Base(base){};
	virtual ~Matrix() = default;
};

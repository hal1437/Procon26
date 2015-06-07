
#pragma once
#include "Block.h"
#include "Point.h"
#include "../Utility/Constants.hpp"
#include <ostream>

struct Transform{
private:
	bool enable;
public:
	Point pos;
	Constants::ANGLE angle;
	bool reverse;

	bool isEnable()const;

	friend std::ostream& operator<<(std::ostream& ost,const Transform& hand);

	Transform():
		enable(false){};
	Transform(const Point& Pos,const Constants::ANGLE& Angle,bool Reverse):
		pos(Pos),
		angle(Angle),
		reverse(Reverse),
		enable(true){};
};




#pragma once
#include "Block.h"
#include "Point.h"
#include "../Utility/Constants.hpp"

struct Hand{
	Block block;
	Point pos;
	Constants::ANGLE angle;
	bool reverse;
	Hand();
	Hand(const Block& b,const Point& Pos,const Constants::ANGLE& Angle,bool Reverse):
		block(b),
		pos(Pos),
		angle(Angle),
		reverse(Reverse){};
};



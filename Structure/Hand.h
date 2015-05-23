
#pragma once
#include "Block.h"
#include "Point.h"
#include "../Utility/Constants.hpp"

struct Hand{
	Block block;
	Point pos;
	Constants::ANGLE angle;
	bool reverse;
};



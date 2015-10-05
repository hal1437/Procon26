
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

public:

	constexpr bool isEnable()const{
		return enable;
	}

	bool operator==(const Transform& rhs)const;
	bool operator<(const Transform& rhs)const;
	friend std::ostream& operator<<(std::ostream& ost,const Transform& hand);

	constexpr Transform():
		pos(Point(0,0)),
		angle(Constants::ANGLE0),
		reverse(false),
		enable(false){}

	constexpr Transform(const Point& Pos,const Constants::ANGLE& Angle,bool Reverse):
		pos(Pos),
		angle(Angle),
		reverse(Reverse),
		enable(true){};
};



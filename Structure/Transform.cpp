
#include "Transform.h"

bool Transform::operator<(const Transform& rhs)const{
	if     (this->pos.x   != rhs.pos.x  )return this->pos.x < rhs.pos.x;
	if     (this->pos.y   != rhs.pos.y  )return this->pos.y < rhs.pos.y;
	else if(this->angle   != rhs.angle  )return this->angle < rhs.angle;
	else if(this->reverse != rhs.reverse)return this->reverse < rhs.reverse;
	else false;
}

std::ostream& operator<<(std::ostream& ost,const Transform& trans){
	if(trans.isEnable() == false) ost << "disable_transform";
	ost << std::boolalpha;
	ost << "(" << trans.pos << "," << static_cast<int>(trans.angle) << "," << trans.reverse << ")";
	return ost;
}




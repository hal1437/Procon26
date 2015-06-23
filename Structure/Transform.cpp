
#include "Transform.h"

bool Transform::isEnable()const{
	return enable;
}

std::ostream& operator<<(std::ostream& ost,const Transform& trans){
	if(trans.isEnable() == false) ost << "disable_transform";
	ost << std::boolalpha;
	ost << "(" << trans.pos << "," << static_cast<int>(trans.angle) << "," << trans.reverse << ")";
	return ost;
}


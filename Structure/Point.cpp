
#include "Point.h"

std::ostream& operator<<(std::ostream& ofs,const Point& point){
	ofs << "(" << point.x << "," << point.y << ")";
	return ofs;
}


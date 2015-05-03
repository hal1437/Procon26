
#pragma once
#include <ostream>

struct Point{
	int x,y;

	Point():x(0),y(0){
	}
	Point(int x,int y):x(x),y(y){
	}

	template<class T>
	inline Point operator*(const T& lhs){
		return {x * lhs,y * lhs};
	}

};

inline std::ostream& operator<<(std::ostream& out,Point pos){
	out << "(" << pos.x << "," << pos.y << ")";
	return out;
}



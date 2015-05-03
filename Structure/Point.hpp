
#pragma once

struct Point{
	int x,y;

	Point():x(0),y(0){
	}

	template<class T>
	inline Point operator*(const T& lhs){
		return {x * lhs,y * lhs};
	}

};


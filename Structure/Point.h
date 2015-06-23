
#pragma once
#include <ostream>

struct Point{
	int x,y;

	Point():x(0),y(0){
	}
	Point(int x,int y):x(x),y(y){
	}

	inline Point operator+(const Point& lhs)const{
		return Point(this->x + lhs.x,this->y + lhs.y);
	}
	inline Point operator-(const Point& lhs)const{
		return Point(this->x - lhs.x,this->y - lhs.y);
	}

	template<class T>
	inline Point operator*(const T& lhs)const{
		return Point(x * lhs,y * lhs);
	}
	template<class T>
	inline Point operator/(const T& lhs)const{
		return Point(x / lhs,y / lhs);
	}
	template<class T>
	inline bool operator==(const T& lhs)const {
		return ((x == lhs.x) && (y==lhs.y));
	}
	template<class T>
	inline bool operator!=(const T& lhs)const {
		return !((*this) == lhs);
	}
	friend std::ostream& operator<<(std::ostream& ofs,const Point& point);
};


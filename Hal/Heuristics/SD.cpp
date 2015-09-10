
#include "SD.h"
#include <cmath>

SD::Point_f operator+(SD::Point_f lhs,SD::Point_f rhs){
	return SD::Point_f(lhs.x + rhs.x,lhs.y + rhs.y);
}
SD::Point_f operator-(SD::Point_f lhs,SD::Point_f rhs){
	return SD::Point_f(lhs.x - rhs.x,lhs.y - rhs.y);
}
SD::Point_f operator*(SD::Point_f lhs,float rhs){
	return SD::Point_f(lhs.x * rhs,lhs.y * rhs);
}
SD::Point_f operator/(SD::Point_f lhs,float rhs){
	return SD::Point_f(lhs.x / rhs,lhs.y / rhs);
}


double SD::Execution(const Field& field){
	Point_f average;
	int count = 0;

	//おもむろに平均を出す
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(!field[i][j]){
				average.x += j;
				average.y += i;
				count++;
			}
		}
	}
	average.x /= count;
	average.y /= count;

	//要素-平均の二乗をわっさーやって数で割ってルート
	Point_f accumulate;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(!field[i][j]){
				accumulate.x += std::pow(j - average.x,2);
				accumulate.y += std::pow(i - average.y,2);
			}
		}
	}
	accumulate.x = accumulate.x / (count-1);
	accumulate.y = accumulate.y / (count-1);

	//std::cout << "--SD--" << std::endl;
	//std::cout << "accumulate.x = " << accumulate.x << std::endl;
	//std::cout << "accumulate.y = " << accumulate.y << std::endl;
	//std::cout << "------" << std::endl;

	return (accumulate.x + accumulate.y);
}

SD::SD(){
}



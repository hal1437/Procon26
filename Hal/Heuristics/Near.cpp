
#include "Near.h"


int Near::FillAround(Field& field,const Point& pos)const{
	field[pos.y][pos.x] = true;
	int n = 1;
	CLOCKWISE_FOR(clock){
		Point seach_point = pos + clock;
		if(seach_point.x < 0 || seach_point.x >= FIELD_WIDTH ||
		   seach_point.y < 0 || seach_point.y >= FIELD_HEIGHT )continue;
		if(field[seach_point.y][seach_point.x] == false){
			n += FillAround(field,seach_point);
		}
	}
	return n;
}

double Near::Execution(const Field& field,const Problem& prob){

	Field _field = field;
	unsigned n = 0;
	std::vector<int> s;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(_field[i][j] == false){
				n++;
				s.push_back(FillAround(_field,Point(j,i)));
			}
		}
	}
	double score=0.0;
	for(int i=0;i<n;i++){
		score += std::max(THRESHOLD - s[i],0);
	}
	return score;
}

Near::Near(){
}






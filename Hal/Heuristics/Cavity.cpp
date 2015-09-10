
#include "Cavity.h"


void Cavity::FillAround(Field& field,const Point& pos)const{
	field[pos.y][pos.x] = true;
	CLOCKWISE_FOR(clock){
		Point seach_point = pos + clock;
		if(seach_point.x < 0 || seach_point.x >= FIELD_WIDTH ||
		   seach_point.y < 0 || seach_point.y >= FIELD_HEIGHT )continue;
		if(field[seach_point.y][seach_point.x] == false){
			FillAround(field,seach_point);
		}
	}
}

double Cavity::Execution(const Field& field){

	Field _field = field;
	unsigned score = 0;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(_field[i][j] == false){
				score++;
				FillAround(_field,Point(j,i));
			}
		}
	}
	return score;
}

Cavity::Cavity(){
}




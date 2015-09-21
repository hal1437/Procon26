
#include "DensityAround.h"


double DensityAround::Execution(const Field& field,const Problem& prob){
	int score = 0;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(field[i][j]){
				score++;
				for(Point pos : {Point(0,1),Point(1,1),Point(1,0),Point(1,-1),Point(0,-1),Point(-1,-1),Point(-1,0),Point(-1,1)}){
					if(j + pos.x < 0 || j + pos.x >= FIELD_WIDTH ||
					   i + pos.y < 0 || i + pos.y >= FIELD_HEIGHT )score++;
					else if(field[i+pos.y][j+pos.x])score++;
				}
			}
		}
	}
	return score;
}

DensityAround::DensityAround(){
}



#include "DensityAround.h"


unsigned DensityAround::Execution(const Field& field){
	int score = 0;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(field[i][j] == false)score++;
			CLOCKWISE_FOR(clock){
				if(j + clock.x < 0 || j + clock.x >= FIELD_WIDTH ||
				   i + clock.y < 0 || i + clock.y >= FIELD_HEIGHT )continue;
				if(field[i+clock.y][j+clock.x])score++;
			}
		}
	}
	return score;
}

DensityAround::DensityAround(){
}


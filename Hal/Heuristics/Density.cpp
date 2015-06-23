
#include "Density.h"

unsigned Density::Execution(const Field& field){
	int score = 0;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(field[i][j])score++;
		}
	}
	return score;
}

Density::Density(){
}



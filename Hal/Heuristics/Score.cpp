
#include "Score.h"

unsigned Score::Execution(const Field& field){
	unsigned score = 0;
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(!field[i][j])score++;
		}
	}
	return score;
}

Score::Score(){
}



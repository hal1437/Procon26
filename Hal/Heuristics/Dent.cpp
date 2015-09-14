
#include "Dent.h"
#include "../../Utility/CLOCKWISE_FOR.hpp"

double Dent::Execution(const Field& field){
	int score = 0;
	
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			Point seach(j,i);
			int around=0;
			CLOCKWISE_FOR(clock){
				Point pos = seach+clock;
				if(pos.x >= 0 && pos.x < FIELD_WIDTH &&
				   pos.y >= 0 && pos.y < FIELD_HEIGHT ){
					if(field[pos.y][pos.x]==true)around++;
				}
			}
			if(around >= 2)score++;
		}
	}
	return score;
}

Dent::Dent(){
}



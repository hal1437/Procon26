
#include "DeadSpace.h"
#include <iostream>

bool DeadSpace::Execution(const Field& field,const BlockLayer& layer){
	for(const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& block : layer){
		if(GetMinLength(block.matrix) <= GetMinLength(Field(~field)))return true;
	}
	return false;
}
MinTriming::MinTriming(){
}


#include "MinTriming.h"
#include <iostream>

bool MinTriming::Execution(const Field& field,const BlockLayer& layer){
	for(const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& block : layer){
		if(GetMinLength(field) >= GetMinLength(block.matrix))return true;
	} 
	return false;
}

MinTriming::MinTriming(){
}

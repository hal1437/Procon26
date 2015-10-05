
#include "MinTriming.h"
#include <iostream>

bool MinTriming::Execution(const Field& field,const BlockLayer& layer){
	Field reach_able = field.GetReachable();
	Field block_field;

	for(const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& block : layer){
		if(GetMinLength(block.matrix) <= GetMinLength(Field(~field)))return true;
	}
	return false;
}
MinTriming::MinTriming(){
}

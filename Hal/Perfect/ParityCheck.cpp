
#include "ParityCheck.h"

bool ParityCheck::Execution(const Field& field,const BlockLayer& layer){
	size_t block_size = 0;
	for(int i=0;i<layer.size();i++){
		block_size += layer[i].matrix.count();
	}
	return (block_size >= field.count());
}

ParityCheck::ParityCheck(){
}

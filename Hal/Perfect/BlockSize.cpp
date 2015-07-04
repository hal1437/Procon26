
#include "BlockSize.h"
#include <iostream>

bool BlockSize::Execution(const Field& field,const BlockLayer& layer){
	size_t block_size = 0;
	for(int i=0;i<layer.size();i++){
		block_size += layer[i].matrix.count();
	}

	return (block_size >= (~field).count());
}

BlockSize::BlockSize(){
}

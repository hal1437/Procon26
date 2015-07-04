
#include "DPBlockSize.h"
#include <iostream>

bool DPBlockSize::Execution(const Field& field,const BlockLayer& layer){
	std::vector<int> block_size;
	std::set<int> dp;

	block_size.resize(layer.size());
	
	for(int i=0;i<layer.size();i++){
		block_size[i] = layer[i].matrix.count();
	}

	const int field_count = (~field).count();
	for(int s:block_size){
		dp.insert(s);
		for(int ss:dp){
			if(s+ss <= field_count){
				dp.insert(s+ss);
			}
		}
	}
	
	return (dp.find(field_count) != dp.end());
}

DPBlockSize::DPBlockSize(){
}

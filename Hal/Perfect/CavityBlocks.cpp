
#include "CavityBlocks.h"
#include <iostream>
#include <set>

int CavityBlocks::FillAround(Field& field,const Point& pos)const{
	int num=1;
	field[pos.y][pos.x] = true;
	CLOCKWISE_FOR(clock){
		Point seach_point = pos + clock;
		if(seach_point.x < 0 || seach_point.x >= FIELD_WIDTH ||
		   seach_point.y < 0 || seach_point.y >= FIELD_HEIGHT )continue;
		if(field[seach_point.y][seach_point.x] == false){
			num += FillAround(field,seach_point);
		}
	}
	return num;
}

bool CavityBlocks::Execution(const Field& field,const BlockLayer& layer){
	size_t cavity_size = 0;
	Field f = field;
	std::multiset<int> size_set;

	//閉空間数 <= ブロック数
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			if(f[i][j] == 0) {
				cavity_size++;
				size_set.insert(FillAround(f,Point(j,i)));
			}
		}
	}
	if(layer.size() <= cavity_size)return false;
/*
	//閉空間サイズはlayerのサイズに含まれる
	for(int i=0;i<layer.size();i++){
		size_set.erase(layer[i].matrix.count());
	}
	return size_set.empty();
*/

	int min_block_size;

	min_block_size = std::min_element(layer.begin(),layer.end(),[](const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& a,const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& b){
		return a.matrix.count() > b.matrix.count();
	})->matrix.count();
	
	return *std::min_element(size_set.begin(),size_set.end()) >= min_block_size;
}

CavityBlocks::CavityBlocks(){
}

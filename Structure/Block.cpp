#include "Block.h"

Block::Block(){
}
Block::Block(Base multi):
	Base(multi)
{

}
Block::~Block(){

}

Block Block::GetRotate(Constants::ANGLE angle)const{
	Block answer;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(angle == Constants::ANGLE90)  answer[i                  ][BLOCK_HEIGHT - j - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE180) answer[BLOCK_WIDTH - j - 1][BLOCK_HEIGHT - i - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE270) answer[BLOCK_WIDTH - i - 1][j]                    = (*this)[j][i];
			if(angle == Constants::ANGLE0)   answer[j                  ][i                   ] = (*this)[j][i];
		}
	}
	return answer;
}
Block Block::GetReverse()const{
	Block answer;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			answer[BLOCK_WIDTH - i - 1][j] = (*this)[j][i];
		}
	}
	return answer;
}


#include "Block.h"

Block::Block(){
	std::fill(begin(),end(),Constants::EMPTY);
}
Block::~Block(){

}

size_t Block::GetSize()const{
	return std::count(begin(),end(),Constants::FILL);
}
Block Block::GetRotate(Constants::ANGLE angle){
	Block answer;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(angle == Constants::ANGLE90)  answer[BLOCK_WIDTH - i - 1][j]                    = (*this)[j][i];
			if(angle == Constants::ANGLE180) answer[BLOCK_WIDTH - j - 1][BLOCK_HEIGHT - i - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE270) answer[i                  ][BLOCK_HEIGHT - j - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE0)   answer[j                  ][i                   ] = (*this)[j][i];
		}
	}
	return answer;
}
Block Block::GetReverse(){
	Block answer;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			answer[BLOCK_HEIGHT - j - 1][i] = (*this)[j][i];
		}
	}
	return answer;
}
std::ostream& operator<<(std::ostream& out,Block block){
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			out << block[j][i];
		}
		out << "\n";
	}
	return out;
}

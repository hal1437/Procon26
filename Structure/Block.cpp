#include "Block.h"

Block::Block(){
	std::fill(begin(),end(),UNUSED);
}
Block::~Block(){

}

size_t Block::GetSize()const{
	return std::count(begin(),end(),ZUKU::USED);
}
Block Block::GetRotate(ANGLE angle){
	Block answer;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(angle == ANGLE90)  answer[BLOCK_WIDTH - i - 1][j]                    = (*this)[j][i];
			if(angle == ANGLE180) answer[BLOCK_WIDTH - j - 1][BLOCK_HEIGHT - i - 1] = (*this)[j][i];
			if(angle == ANGLE270) answer[i                  ][BLOCK_HEIGHT - j - 1] = (*this)[j][i];
			if(angle == ANGLE0)   answer[j                  ][i                   ] = (*this)[j][i];
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

#include "Block.h"

Block::Block(){
	std::fill(begin(),end(),Constants::EMPTY);
}
Block::Block(std::initializer_list<std::initializer_list<int>> init){
	int i,j;
	i=0;
	for(std::initializer_list<int> list : init){
		j=0;
		for(int x : list){
			(*this)[i][j] = static_cast<Constants::ZUKU>(x);
			j++;
		}
		i++;
	}
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
			if(angle == Constants::ANGLE90)  answer[i                  ][BLOCK_HEIGHT - j - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE180) answer[BLOCK_WIDTH - j - 1][BLOCK_HEIGHT - i - 1] = (*this)[j][i];
			if(angle == Constants::ANGLE270) answer[BLOCK_WIDTH - i - 1][j]                    = (*this)[j][i];
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
			out << block[i][j];
		}
		out << "\n";
	}
	return out;
}

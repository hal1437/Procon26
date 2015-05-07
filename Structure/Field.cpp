
#include "Field.h"

Field::Field(){
	std::fill(begin(),end(),Constants::EMPTY);
}

Field::Field(std::initializer_list<std::initializer_list<int>> init){
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
Field::~Field(){

}

void Field::Projection(const Point pos,const Block block){
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(pos.y + i < 0 || pos.y + i >= FIELD_WIDTH ||
			   pos.x + j < 0 || pos.x + j >= FIELD_HEIGHT)continue;
			if(block[i][j] == Constants::FILL)(*this)[pos.y + i][pos.x + j] = Constants::FILL;
		}
	}
}
bool Field::isLayPossible(const Point pos,const Block block){
	bool adjacent = false;
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(block[i][j] == Constants::FILL){
				//overrun
				if(pos.y+i < 0 || pos.x+j < 0 || pos.y+i >= FIELD_HEIGHT || pos.x+j >= FIELD_WIDTH){
					std::cout << "overrun:(" << pos.x + j << "," <<  pos.y + i << ")\n";
					return false;
				}
				//crossed
				if((*this)[pos.y + i][pos.x + j] == Constants::FILL){
					std::cout << "crossed:(" << pos.x + j << "," <<  pos.y + i << ")\n";
					return false;
				}
				//not Adjacent
				for(Point clockwise :{Point(0,1),Point(1,0),Point(0,-1),Point(-1,0)}){
					Point seach_point = pos + Point(j,i) + clockwise;
					//overrun
					if(seach_point.y < 0 || seach_point.y < 0 || seach_point.y >= BLOCK_HEIGHT || seach_point.x >= BLOCK_WIDTH){
						continue;
					}
					//exist
					if((*this)[seach_point] == Constants::FILL)adjacent = true;
				}
			}
		}
	}
	return adjacent;
}


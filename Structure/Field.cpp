
#include "Field.h"

Field::Field(){
}
Field::Field(Base multi):
	Base(multi)
{
}
Field::~Field(){

}

void Field::Projection(const Point pos,const Block block){
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(pos.y + i < 0 || pos.y + i >= FIELD_WIDTH ||
			   pos.x + j < 0 || pos.x + j >= FIELD_HEIGHT)continue;
			if(get(j,i) == Constants::FILL)set(pos.x + j,pos.y + i,true);
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
				if(get(pos.x + j,pos.y + i)){
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
					if(get(seach_point.x,seach_point.y))adjacent = true;
				}
			}
		}
	}
	return adjacent;
}


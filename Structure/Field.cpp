
#include "Field.h"

Field::Field(){
	std::fill(begin(),end(),0);
}
Field::Field(Base multi):
	Base(multi)
{
}
Field::~Field(){

}

int Field::GetCount()const{
	return block_count;
}

void Field::Projection(const Point pos,const Block block){
	block_count++;
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(pos.y + i < 0 || pos.y + i >= FIELD_WIDTH ||
			   pos.x + j < 0 || pos.x + j >= FIELD_HEIGHT)continue;
			if(block[i][j] == Constants::FILL)Base::operator[](pos.y + i)[pos.x + j] = block_count;
		}
	}
}
bool Field::isLayPossible(const Point pos,const Block block)const{
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
				if(Base::operator[](Point(pos.x + j,pos.y + i)) != 0){
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
					if(Base::operator[](seach_point) > 1)adjacent = true;
				}
			}
		}
	}
	//first bolock
	if(block_count==1)return true;
	return adjacent;
}

Constants::ZUKU* Field::operator[](const size_t& index){
	return reinterpret_cast<Constants::ZUKU*>(&Base::operator[](index)[0]);
}
const Constants::ZUKU* Field::operator[](const size_t& index)const{
	return reinterpret_cast<const Constants::ZUKU*>(&Base::operator[](index)[0]);
}


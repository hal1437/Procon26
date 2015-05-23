
#include "Field.h"

Field::Field(){
}
Field::Field(Base multi):
	Base(multi)
{
}
Field::~Field(){

}

std::vector<Hand> Field::GetListLayPossible(const Block& block)const{
	std::vector<Hand> answer;
	Block blocks[2][4];

	for(int i=0;i<2;i++){
		for(int j=0;j<4;j++){
			blocks[i][j] = ((i==0) ? block : block.GetReverse()).GetRotate(static_cast<Constants::ANGLE>(j));
		}
	}
	for(int i=-7;i<FIELD_HEIGHT;i++){
		for(int j=-7;j<FIELD_WIDTH;j++){
			for(int k=0;k<4;k++){
				for(int r=0;r<2;r++){
					if(isLayPossible(Point(j,i),blocks[r][k]))answer.push_back(Hand{block,
																					Point(j,i),
																					static_cast<Constants::ANGLE>(k),
																					static_cast<bool>(r)});
				}
			}
		}
	}
	return answer;
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
bool Field::isLayPossible(const Point pos,const Block block)const{
	bool adjacent = false;
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(block[i][j] == Constants::FILL){
				//overrun
				if(pos.y+i < 0 || pos.x+j < 0 || pos.y+i >= FIELD_HEIGHT || pos.x+j >= FIELD_WIDTH){
					//std::cout << "overrun:(" << pos.x + j << "," <<  pos.y + i << ")\n";
					return false;
				}
				//crossed
				if(get(pos.x + j,pos.y + i)){
					//std::cout << "crossed:(" << pos.x + j << "," <<  pos.y + i << ")\n";
					return false;
				}
				//not Adjacent
				CLOCKWISE_FOR(clockwise){
					Point seach_point = pos + Point(j,i) + clockwise;
					//overrun
					if(seach_point.x < 0 || seach_point.y < 0 || seach_point.y >= BLOCK_HEIGHT || seach_point.x >= BLOCK_WIDTH){
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


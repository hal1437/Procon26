
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
			blocks[i][j] = ((i==0) ? block : block.GetReverse()).GetRotate(static_cast<Constants::ANGLE>(j*90));
		}
	}


	for(int i=-8;i<FIELD_HEIGHT;i++){
		for(int j=-8;j<FIELD_WIDTH;j++){
			for(int k=0;k<4;k++){
				for(int r=0;r<2;r++){
					if(isLayPossible(Point(j,i),blocks[r][k])){
						answer.push_back(Hand{block,
											Point(j,i),
											static_cast<Constants::ANGLE>(k*90),
											static_cast<bool>(r)});
					}
				}
			}
		}
	}
	return answer;
}

void Field::Projection(const Point& pos,const Block& block){
	Projection(Hand(block,pos,Constants::ANGLE0,false));
}
void Field::Projection(const Hand& hand){
	Block block = ((hand.reverse) ? hand.block.GetReverse() : hand.block).GetRotate(hand.angle);
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(hand.pos.y + i < 0 || hand.pos.y + i >= FIELD_WIDTH ||
			   hand.pos.x + j < 0 || hand.pos.x + j >= FIELD_HEIGHT)continue;
			if(block.get(j,i) == true)set(hand.pos.x + j,hand.pos.y + i,true);
		}
	}
	hands.push_back(hand);
}


bool Field::isLayPossible(const Point pos,const Block block)const{
	bool adjacent = false;
	for(int i = 0;i < BLOCK_HEIGHT;i++){
		for(int j = 0;j < BLOCK_WIDTH;j++){
			if(block[i][j]){
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
					for(Hand hand : hands){
						Point local_pos = seach_point - hand.pos;
						if(local_pos.y < 0 || local_pos.x < 0 || local_pos.y >= BLOCK_HEIGHT || local_pos.x >= BLOCK_WIDTH){
							continue;
						}else{
							if((hand.reverse ? hand.block.GetReverse() : hand.block).GetRotate(hand.angle).get(local_pos.x,local_pos.y)){
								adjacent = true;
								//std::cout << local_pos << std::endl;
								//std::cout << "t";
							}
						}
					} 
				}
			}
		}
	}
	if(hands.size()==0)return true;
	//if(adjacent) std::cout << "ok";
	return adjacent;
}


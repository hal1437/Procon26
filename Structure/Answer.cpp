
#include "Answer.h"
#include <fstream>

void Answer::SetField (Field fi){
	this->field = fi;
}
void Answer::AddBlocks(){
	hands.push_back(Hand(Block(),Point(-1,-1),Constants::ANGLE0,false));
}
void Answer::AddBlocks(Block block,Point pos,bool reverse,Constants::ANGLE angle){
	hands.push_back(Hand{block,pos,angle,reverse});
}
bool Answer::Export(std::string filename)const{
	std::ofstream ofs(filename);
	if(!ofs)return false;
	ofs << (this);
	return true;
}

Field Answer::GetField(){
	Field field = this->field;
	for(Hand& hand : hands){
		if(hand.pos != Point(-1,-1))field.Projection(hand.pos,hand.block);
	}
	return field;
}

std::ostream& operator<<(std::ostream& ost,const Answer& answer){
	for(int i=0;i < answer.hands.size();i++){
		if(hand.pos != Point(-1,-1)){
			ost << hands[i].pos.x;
			ost << " ";
			ost << hands[i].pos.y;
			ost << " ";
			ost << (hands[i].reverse ? "T" : "H");
			ost << " ";
			ost << hands[i].angle;
		}
		if(i != hands.size()-1)ost << "\r\n";
	}

	return ost;
}


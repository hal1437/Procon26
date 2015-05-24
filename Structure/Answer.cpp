
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
	for(Hand hand : answer.hands){
		if(hand.pos != Point(-1,-1)){
			ost << hand.pos.x;
			ost << " ";
			ost << hand.pos.y;
			ost << " ";
			ost << (hand.reverse ? "T" : "H");
			ost << " ";
			ost << hand.angle;
		}
		ost << "\r\n";
	}

	return ost;
}


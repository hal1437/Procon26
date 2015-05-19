
#include "Answer.h"
#include <fstream>

void Answer::SetField (Field fi){
	this->field = fi;
}
void Answer::AddBlocks(){
	hands.push_back(Hand{Block(),Point(Invaild,Invaild)});
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
	for(Answer::Hand& hand : hands){
		if(hand.pos != Point(Invaild,Invaild)){
			Block block;
			if(hand.reverse)block = hand.block.GetReverse();
			else            block = hand.block;
			field.Projection(hand.pos,block.GetRotate(hand.angle));
		}
	}
	return field;
}

std::ostream& operator<<(std::ostream& ost,const Answer& answer){
	for(Answer::Hand hand : answer.hands){
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


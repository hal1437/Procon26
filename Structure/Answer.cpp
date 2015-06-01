
#include "Answer.h"
#include <fstream>

void Answer::SetField (Field fi){
	this->field = fi;
}
void Answer::AddBlocks(){
	hands.push_back(Hand(Block(),Point(-1,-1),Constants::ANGLE0,false));
}
void Answer::AddBlocks(Hand hand){
	if(hand.block.count()==0)hands.push_back(Hand(Block(),Point(-1,-1),Constants::ANGLE0,false));
	else hands.push_back(hand);
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
		if(answer.hands[i].pos != Point(-1,-1)){
			ost << answer.hands[i].pos.x;
			ost << " ";
			ost << answer.hands[i].pos.y;
			ost << " ";
			ost << (answer.hands[i].reverse ? "T" : "H");
			ost << " ";
			ost << answer.hands[i].angle;
		}
		if(i != answer.hands.size()-1)ost << "\r\n";
	}

	return ost;
}




#include "Answer.h"
#include <fstream>

void Answer::SetField (Field fi){
	this->field = fi;
}
void Answer::AddBlocks(){
	transes.push_back(Transform());
}
void Answer::AddBlocks(Transform trans){
	transes.push_back(trans);
}
void Answer::AddBlocks(Point pos,bool reverse,Constants::ANGLE angle){
	transes.push_back(Transform(pos,angle,reverse));
}
bool Answer::Export(std::string filename)const{
	std::ofstream ofs(filename);
	if(!ofs)return false;
	ofs << (this);
	return true;
}

Field Answer::GetField()const{
	Field field = this->field;
	for(int i=0;i<transes.size();i++){
		if(transes[i].isEnable() != false)field.Projection(problem.GetBlock(i),transes[i]);
	}
	return field;
}

std::ostream& operator<<(std::ostream& ost,const Answer& answer){
	for(int i=0;i < answer.transes.size();i++){
		if(answer.transes[i].isEnable()){
			ost << answer.transes[i].pos.x;
			ost << " ";
			ost << answer.transes[i].pos.y;
			ost << " ";
			ost << (answer.transes[i].reverse ? "T" : "H");
			ost << " ";
			ost << answer.transes[i].angle;
		}
		if(i != answer.transes.size()-1)ost << "\r\n";
	}

	return ost;
}


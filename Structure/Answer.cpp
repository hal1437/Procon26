

#include "Answer.h"
#include <fstream>

 void Answer::SetProblem(const Problem& prob){
	this->resize(prob.Count());
	initial_field = prob.GetField();
	for(int i=0;i<prob.Count();i++){
		(*this)[i].matrix = prob.GetBlock(i);
	}
}
void Answer::SetBlock(size_t index,const Block& block){
	(*this)[index].matrix = block;
}
void Answer::SetTransform(size_t index,const Transform& trans){
	(*this)[index].trans = trans;
}
void Answer::SetField (const Field& field){
	this->initial_field = field;
}
Field& Answer::GetField(){
	return this->initial_field;
}
Transform& Answer::GetTransform(size_t index){
	return (*this)[index].trans;
}
Block& Answer::GetBlock(size_t index){
	return (*this)[index].matrix;
}
Field Answer::GetProjectedField()const{
	Field field = this->initial_field;
	for(int i=0;i<this->size();i++){
		if((*this)[i].trans.isEnable() != false)field.Projection((*this)[i].matrix,(*this)[i].trans);
	}
	return field;
}

bool Answer::Export(std::string filename)const{
	std::ofstream ofs(filename);
	if(!ofs)return false;
	ofs << (*this) << std::endl;
	return true;
}

std::ostream& operator<<(std::ostream& ost,const Answer& answer){
	std::cout << answer.size() << std::endl;
	for(int i=0;i<answer.size();i++){
		if(answer[i].trans.isEnable()){
			ost << answer[i].trans.pos.x;
			ost << " ";
			ost << answer[i].trans.pos.y;
			ost << " ";
			ost << (answer[i].trans.reverse ? "T" : "H");
			ost << " ";
			ost << answer[i].trans.angle;
		}
		if(i != answer.size()-1)ost << "\r\n";
	}
	return ost;
}
Answer::Answer(){
}
Answer::Answer(const Problem& prob){
	SetProblem(prob);
}
Answer::~Answer(){
}



#include "Problem.h"
#include <fstream>
#include <sstream>

std::string Problem::ToString()const{
	std::stringstream ss;

	ss << field;
	ss << "\r\n";
	ss << Base::size() << "\r\n";
	
	for(int i = 0;i < Base::size();i++){
		ss << (*this)[i].matrix;
		if(i != Base::size()-1)ss << "\r\n";
	}
	return ss.str();
}

//file input output
bool Problem::Export(std::string filename)const{
	std::ofstream ost(filename);
	if(!ost)return false;
	ost << field;
	ost << "\r\n";
	ost << Base::size() << "\r\n";
	for(Base::const_iterator it = Base::begin();it != Base::end();it++){
		ost << it->matrix;
		if(it != Base::end()-1)ost << "\r\n";
	}
	return true;
}
bool Problem::Inport(std::string filename){
	std::ifstream ist(filename);
	int nums;
	std::string str;
	
	if(!ist)return false;
	ist >> field;
	std::getline(ist,str);
	ist >> nums;
	std::getline(ist,str);
	Base::resize(nums);
	for(int i=0;i<nums;i++){
		ist >> (*this)[i].matrix;
		if(i != nums-1)std::getline(ist,str);
	}
	return true;
}

//getter
const Field& Problem::GetField()const{
	return field;
}
const Block& Problem::GetBlock(size_t index)const{
	return (*this)[index].matrix;
}
size_t Problem::Count()const{
	return Base::size();
}

//constructor
Problem::Problem(){
}
Problem::Problem(std::string filename){
	Inport(filename);
}
Problem::~Problem(){
}

std::ostream& operator<<(std::ostream& ost,const Problem& prob){
	ost << prob.ToString();
	return ost;
}


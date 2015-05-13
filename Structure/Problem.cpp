
#include "Problem.h"
#include <fstream>
#include <sstream>

std::string Problem::ToString()const{
	std::stringstream ss;

	ss << field;
	ss << "\r\n";
	ss << blocks.size() << "\r\n";
	
	for(int i = 0;i < blocks.size();i++){
		ss << blocks[i];
		if(i != blocks.size()-1)ss << "\r\n";
	}
	return ss.str();
}

//file input output
bool Problem::Export(std::string filename)const{
	std::ofstream ost(filename);
	if(!ost)return false;
	ost << field;
	ost << "\r\n";
	ost << blocks.size() << "\r\n";
	for(std::vector<Block>::const_iterator it = blocks.begin();it != blocks.end();it++){
		ost << *it;
		if(it != blocks.end()-1)ost << "\r\n";
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
	blocks.resize(nums);
	for(int i=0;i<nums;i++){
		ist >> blocks[i];
		if(i != nums-1)std::getline(ist,str);
	}
	return true;
}

//getter
Field Problem::GetField()const{
	return field;
}
Block Problem::GetBlock(size_t index)const{
	return blocks[index];
}
size_t Problem::Count()const{
	return blocks.size();
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


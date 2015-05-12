
#include "Problem.h"
#include <ofstream>
#include <ifstream>
#include <sstream>

std::string Problem::ToString()const{
	std::sstream ss;

	ss << field;
	ss << "\r\n";
	ss << blocks.size() << "\r\n";
	
	for(int i = 0;i < blocks.size();i++){
		ss << block[i];
		if(i != blocks.size()-1)ss << "\r\n";
	}
	return ss.str();
}

//file input output
virtual bool Problem::Export(std::string filename)const{
	std::ofstream ost(filename);
	if(!ost)return false;
	ost << ToString();
	return true;
}
virtual bool Inport(std::string filename)const{
	std::ifstream ifs(filename);
	if(!ifs)return false;

	if()
}

//getter
Field GetField()const;
Block GetBlock(size_t index)const;

//constructor
Problem();
Problem(std::string filename);
virtual ~Problem();

std::ostream operator<<(std::ostream& ost,const Problem& prob){
	
}


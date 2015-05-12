
#pragma once
#include "Field.h"
#include <vector>
#include <string>

class Problem{
protected:
	Field field;
	std::vector<Block> blocks;

public:
	
	//convert to std::string
	virtual std::string ToString()const;
	
	//file input output
	virtual bool Export(std::string filename)const;
	virtual bool Inport(std::string filename)const;

	//getter
	Field GetField()const;
	Block GetBlock(size_t index)const;

	//constructor
	Problem();
	Problem(std::string filename);
	virtual ~Problem();

	friend std::ostream operator<<(std::ostream& ost,const Problem& prob);

};

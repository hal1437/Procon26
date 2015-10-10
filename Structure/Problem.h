
#pragma once
#include "Layer.hpp"
#include <vector>
#include <string>

class Problem:protected Layer<BLOCK_WIDTH,BLOCK_HEIGHT>{
private:
	typedef Layer<BLOCK_WIDTH,BLOCK_HEIGHT> Base;
protected:
	Field field;


public:
	
	//convert to std::string
	virtual std::string ToString()const;
	
	//file input output
	virtual bool Export(std::string filename)const;
	virtual bool Inport(std::string filename);

	//getter
	Field GetField()const;
	Block GetBlock(size_t index)const;
	size_t Count()const;

	//constructor
	Problem();
	Problem(std::string filename);
	virtual ~Problem();

	friend std::ostream& operator<<(std::ostream& ost,const Problem& prob);

};

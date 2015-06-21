
#pragma once

#include "Layer.hpp"
#include "Problem.h"
#include <vector>
#include <string>

class Answer:protected Layer<BLOCK_WIDTH,BLOCK_HEIGHT>{
private:
	typedef Layer<BLOCK_WIDTH,BLOCK_HEIGHT> Base;
	Field initial_field;

public:
	void SetProblem(const Problem& prob);
	void SetField (const Field& field);
	void SetBlock(size_t index,const Block& block);
	void SetTransform(size_t index,const Transform& trans);
	Field&     GetField();
	Block&     GetBlock(size_t index);
	Transform& GetTransform(size_t index);
	Field      GetProjectedField()const;


	bool Export(std::string filename)const;

	Answer();
	Answer(const Problem& prob);
	~Answer();

	friend std::ostream& operator<<(std::ostream& ost,const Answer& answer);
};


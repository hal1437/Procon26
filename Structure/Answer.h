
#pragma once

#include "Transform.h"
#include "Matrix.hpp"
#include "Problem.h"
#include <vector>
#include <string>

class Answer{
private:
	const Problem& problem;
	Field field;
	std::vector<Transform> transes;

public:
	void SetField (Field filed);
	void AddBlocks();
	void AddBlocks(Transform trans);
	void AddBlocks(Point pos,bool reverse,Constants::ANGLE angle);

	bool Export(std::string filename)const;
	Field GetField()const;

	Answer(const Problem& prob):problem(prob){};
	~Answer(){};

	friend std::ostream& operator<<(std::ostream& ost,const Answer& answer);
};



#pragma once

#include "Hand.h"
#include "Field.h"
#include <vector>
#include <string>

class Answer{
private:
	Field field;
	std::vector<Hand> hands;

public:
	void SetField (Field filed);
	void AddBlocks();
	void AddBlocks(Block block,Point pos,bool reverse,Constants::ANGLE angle);

	bool Export(std::string filename)const;
	Field GetField();

	Answer(){};
	~Answer(){};

	friend std::ostream& operator<<(std::ostream& ost,const Answer& answer);
};


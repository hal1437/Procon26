
#pragma once

#include "Field.h"
#include "Block.h"
#include "Point.hpp"
#include <vector>
#include <string>

class Answer{
public:
	struct Hand{
		Block block;
		Point pos;
		Constants::ANGLE angle;
		bool reverse;
	};
private:
	const int Invaild = 10;
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


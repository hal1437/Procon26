
#pragma once
#include "Multi.hpp"
#include "Block.h"
#include "Point.hpp"
#include <initializer_list>
#include <ostream>
#include <vector>
#include "../Utility/Constants.hpp"
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : public Multi<int,FIELD_HEIGHT,FIELD_WIDTH>{
protected:
	typedef Multi<int,FIELD_HEIGHT,FIELD_WIDTH> Base;
	int block_count=1;

public:

	int  GetCount()const;
	void Projection   (const Point pos,const Block block);		//Added block to field
	bool isLayPossible(const Point pos,const Block block)const;	//Checking cross Block
	std::vector<Point> GetLayList(Block block);

	Constants::ZUKU* operator[](const size_t& index);
	const Constants::ZUKU* operator[](const size_t& index)const;
	Field();
	Field(Base multi);
	virtual ~Field();
};


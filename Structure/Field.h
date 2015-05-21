
#pragma once
#include "MultiBit.hpp"
#include "Block.h"
#include "Point.hpp"
#include <initializer_list>
#include <ostream>
#include "../Utility/Constants.hpp"
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : public MultiBit<FIELD_HEIGHT,FIELD_WIDTH>{
private:
	typedef MultiBit<FIELD_HEIGHT,FIELD_WIDTH> Base;

public:

	void Projection   (const Point pos,const Block block);	//Added block to field
	bool isLayPossible(const Point pos,const Block block);	//Checking cross Block

	Field();
	Field(Base multi);
	virtual ~Field();
};


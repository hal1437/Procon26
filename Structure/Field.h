
#pragma once
#include "Multi.hpp"
#include "Block.h"
#include "Point.hpp"
#include <initializer_list>
#include <ostream>
#include "../Utility/Constants.hpp"
#include <initializer_list>
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : public Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH>{
private:
	typedef Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH> Base;

public:

	void Projection   (const Point pos,const Block block);	//Added block to field
	bool isLayPossible(const Point pos,const Block block);	//Checking cross Block

	Field();
	Field(Base multi);
	Field(std::initializer_list<std::initializer_list<int>> init);	
	virtual ~Field();
};



#pragma once
#include "Multi.hpp"
#include "Block.h"
#include "Point.hpp"
#include "../Utility/Constants.hpp"
#include <initializer_list>
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : public Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH>{
private:
	typedef Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH> Base;

public:

	Field();
    Field(std::initializer_list<std::initializer_list<int>> init);
    virtual ~Field();

	void Projection(const Point pos,const Block block);	//Added block to field
	bool isLayPossible(const Point pos,const Block block);	//Checking cross Block
	
};



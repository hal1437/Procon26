
#pragma once
#include "Multi.hpp"
#include "Block.h"
#include "Point.hpp"
#include <initializer_list>
#include "../Utility/Constants.hpp"
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : private Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH>{
private:
	typedef Multi<Constants::ZUKU,FIELD_HEIGHT,FIELD_WIDTH> Base;

public:
	using Base::iterator;
	using Base::child;
	using Base::begin;
	using Base::end;
	using Base::operator[];

	Field();
	Field(std::initializer_list<std::initializer_list<int>> init);
	virtual ~Field();

	void Projection(Point Pos,Block block);	//Added block to field
	bool isCross   (Point Pos,Block block);	//Checking cross Block

};

std::ostream& operator<<(std::ostream& out,Field field);


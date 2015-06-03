
#pragma once
#include "MultiBit.hpp"
#include "../Utility/Constants.hpp"
#include <iostream>
#include <algorithm>
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8

class Block : public MultiBit<BLOCK_HEIGHT,BLOCK_WIDTH>{
private:
	typedef MultiBit<BLOCK_HEIGHT,BLOCK_WIDTH> Base;

public:
	//using Base::iterator;
	//using Base::child;
	//using Base::begin;
	//using Base::end;
	using Base::operator[];
	using Base::operator=;

	Block();
	Block(Base multi);
	virtual ~Block();

	Block  GetRotate(Constants::ANGLE angle)const; 	//Get rotated piece
	Block  GetReverse()const;						//Get reversed piece
};



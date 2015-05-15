
#pragma once
#include "Multi.hpp"
#include "../Utility/Constants.hpp"
#include <iostream>
#include <initializer_list>
#include <algorithm>
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8

class Block : public Multi<Constants::ZUKU,BLOCK_HEIGHT,BLOCK_WIDTH>{
private:
	typedef Multi<Constants::ZUKU,BLOCK_HEIGHT,BLOCK_WIDTH> Base;

public:
	using Base::iterator;
	using Base::child;
	using Base::begin;
	using Base::end;
	using Base::operator[];

	Block();
	Block(Base multi);
	virtual ~Block();

	size_t GetSize()const; 			 			//size of pieces [zk]
	Block  GetRotate(Constants::ANGLE angle); 	//Get rotated piece
	Block  GetReverse();						//Get reversed piece
};



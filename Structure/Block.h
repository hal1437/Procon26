
#pragma once
#include "Multi.hpp"
#include <iostream>
#include <algorithm>
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8

enum ZUKU{
	UNUSED = 0,
	USED   = 1,
};
enum ANGLE{
	ANGLE0,
	ANGLE90,
	ANGLE180,
	ANGLE270,
};

class Block : private Multi<ZUKU,BLOCK_HEIGHT,BLOCK_WIDTH>{
private:
	typedef Multi<ZUKU,BLOCK_HEIGHT,BLOCK_WIDTH> Base;

public:
	using Base::iterator;
	using Base::child;
	using Base::begin;
	using Base::end;
	using Base::operator[];

	explicit Block();
	virtual ~Block();

	size_t GetSize()const; 			 	//size of pieces [zk]
	Block  GetRotate(ANGLE angle); //Get rotated piece
	Block  GetReverse();			//Get reversed piece
};

std::ostream& operator<<(std::ostream& out,Block block);



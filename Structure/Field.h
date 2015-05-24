
#pragma once
#include "MultiBit.hpp"
#include "Hand.h"
#include <initializer_list>
#include <vector>
#include <ostream>
#include "../Utility/Constants.hpp"
#include "../Utility/CLOCKWISE_FOR.hpp"
#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32

class Field : public MultiBit<FIELD_HEIGHT,FIELD_WIDTH>{
private:
	typedef MultiBit<FIELD_HEIGHT,FIELD_WIDTH> Base;
	std::vector<Hand> hands;

public:

	std::vector<Hand> GetListLayPossible(const Block& block)const;
	void Projection   (const Point& pos,const Block& block);	//Added block to field
	void Projection   (const Hand& block);						//Added block to field
	bool isLayPossible(const Point pos,const Block block)const;	//Checking cross Block

	Field();
	Field(Base multi);
	virtual ~Field();
};


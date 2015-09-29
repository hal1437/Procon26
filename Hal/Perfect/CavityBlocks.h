
#pragma once
#include "../Structures.h"

class CavityBlocks : public Perfect<Field,BlockLayer>{
private:
	typedef Perfect<Field,BlockLayer> h_type;

private:
	int FillAround(Field& field,const Point& pos)const;

public:


	bool Execution(const Field& field,const BlockLayer& layer);

	CavityBlocks();
};


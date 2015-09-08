
#pragma once
#include "../Structures.h"
#include "../Characteristics.h"

class PPPP : public Perfect<Field,BlockLayer>{
private:
	typedef Perfect<Field,BlockLayer> h_type;
	
public:

	std::vector<Characteristics> chars;

	bool Execution(const Field& field,const BlockLayer& layer);

	PPPP();
};


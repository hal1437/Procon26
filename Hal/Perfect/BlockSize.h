
#pragma once
#include "../Structures.h"

class BlockSize : public Perfect<Field,BlockLayer>{
private:
	typedef Perfect<Field,BlockLayer> h_type;
public:

	bool Execution(const Field& field,const BlockLayer& layer);

	BlockSize();
};


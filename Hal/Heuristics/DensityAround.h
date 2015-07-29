
#pragma once
#include "../Structures.h"

class DensityAround : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	DensityAround();
};

/*
template<size_t AROUND>
class DensityAroundMore : public Heuristics<unsigned,Field>{
public:
	unsigned Execution(const Field& field);

	DensityAround();
};



template<size_t AROUND>
unsigned DensityAroundMore::Execution(const Field& field){


	
}
*/


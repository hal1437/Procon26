#include "Random.h"

double Random::Execution(const Field& field,const Problem& prob){
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return std::generate_canonical<double, 10>(gen);
}


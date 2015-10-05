
#include "ParityCheck.h"

ParityCheck::Parity ParityCheck::Parity::operator+(const Parity& rhs)const{
	return ParityCheck::Parity(this->odd + this->even , rhs.odd + rhs.even);
}

bool ParityCheck::Parity::isEqualParity()const{
	return (odd == even);
}

bool ParityCheck::Parity::operator<(const Parity& rhs)const{
	return ((this->odd + this->even) < (rhs.odd + rhs.even));
}

bool ParityCheck::Execution(const Field& field,const BlockLayer& layer){

	std::set<Parity> p_sets;
	Parity field_parity = GetParity(field);

	//偶数奇数調べるあれ
	for(const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT> state:layer){
		if(field_parity.isEqualParity() == GetParity(state.matrix).isEqualParity())return true;
	}
	
	
	return false;
}

ParityCheck::ParityCheck(){
}




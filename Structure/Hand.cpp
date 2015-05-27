
#include "Hand.h"

Block Hand::operator()()const{
	Block block = this->block;
	if(reverse)block = block.GetReverse();
	block = block.GetRotate(angle);
	return block;
}

std::ostream& operator<<(std::ostream& ost,const Hand& hand){
	ost << std::boolalpha;
	ost << hand.pos << "," << static_cast<int>(hand.angle) << "," << hand.reverse << std::endl;
	ost << hand.block << std::endl;
	return ost;
}



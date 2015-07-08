
#include "ParityCheck.h"

ParityCheck::Parity ParityCheck::Parity::operator+(const Parity& rhs)const{
	return ParityCheck::Parity(this->odd + this->even , rhs.odd + rhs.even);
}

bool ParityCheck::Parity::operator<(const Parity& rhs)const{
	return ((this->odd + this->even) < (rhs.odd + rhs.even));
}

bool ParityCheck::Execution(const Field& field,const BlockLayer& layer){
	//偶奇数によって動的計画法を行う。
	//DPBlockSizeとは完全上位互換になる
	
	std::set<Parity> p_sets;
	Parity field_parity = GetParity(field);

	//ブロック抽出
	for(const BlockState<BLOCK_WIDTH,BLOCK_HEIGHT>& block : layer){
		p_sets.insert(GetParity(block.matrix));
		//std::cout << p_sets.size() << std::endl;
		
		//全てのブロックに加算する。
		int p_sets_size = p_sets.size();
		for(const Parity& ss :p_sets){
			if(ss.odd < field_parity.odd+1 && ss.even < field_parity.even+1){
				p_sets.insert(ss + GetParity(block.matrix));
			}
		}
		
	}

	for(const Parity& p :p_sets){
		//std::cout << "odd:" << p.odd << " even:" << p.even << " size:" << p.odd+p.even << std::endl;
	}
	return true;
}

ParityCheck::ParityCheck(){
}

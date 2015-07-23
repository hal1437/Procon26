
#pragma once
#include "../Structures.h"
#include <set>

class ParityCheck : public Perfect<Field,BlockLayer>{
public:
	typedef Perfect<Field,BlockLayer> h_type;
	
	struct Parity{
		int odd;	//奇数
		int even;	//偶数


		bool isEqualParity()const;
		Parity operator+(const Parity& rhs)const;
		bool   operator<(const Parity& rhs)const;
		constexpr Parity():odd(0),even(0){};
		constexpr Parity(int Odd,int Even):odd(Odd),even(Even){};
	};
private:

	template<size_t WIDTH,size_t HEIGHT>
	constexpr Parity GetParity(const Matrix<WIDTH,HEIGHT>& matrix)const;

	template<size_t WIDTH,size_t HEIGHT>
	constexpr Matrix<WIDTH,HEIGHT> CreateCheckeredPattern()const;

public:

	bool Execution(const Field& field,const BlockLayer& layer);

	ParityCheck();
};


template<size_t WIDTH,size_t HEIGHT>
constexpr ParityCheck::Parity ParityCheck::GetParity(const Matrix<WIDTH,HEIGHT>& matrix)const{
	Matrix<WIDTH,HEIGHT> mask = CreateCheckeredPattern<WIDTH,HEIGHT>();
	Matrix<WIDTH,HEIGHT> mask_r = ~mask;

	return Parity((matrix & mask).count(),(matrix & mask_r).count());
}

template<size_t WIDTH,size_t HEIGHT>
constexpr Matrix<WIDTH,HEIGHT> ParityCheck::CreateCheckeredPattern()const{
	Matrix<WIDTH,HEIGHT> ans;
	for(int i=0;i<WIDTH;i++){
		for(int j=0;j<HEIGHT;j++){
			if((i+j) % 2 == 0){
				ans[i][j]=1;
			}
		}
	}
	return ans;
}



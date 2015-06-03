
#pragma once
#include "MultiBit.hpp"
#include "Point.h"
#include "Transform.h"
#include "../Utility/Constants.hpp"

#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8
#define MATRIX_SIZE_ASSERT static_assert(MATRIX_WIDTH<ARGS_WIDTH || MATRIX_HEIGHT<ARGS_HEIGHT,"MATRIX_SIZE_ASSERT")

template<size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class Matrix:public MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>{
private:
	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> Base;
	typedef Matrix<MATRIX_WIDTH,MATRIX_HEIGHT> current;

public:

	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
		(*this) |= current(mat);
		return (*this);
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,Transform trans){
		return Projection(Matrix<ARGS_WIDTH,ARGS_HEIGHT>(mat).Transform(trans));
	}

	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix){
		return (((*this) & matrix.count) > 0);
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Point& pos){
		return Cross(matrix.Move(pos));
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Transform& hand){
		return Cross(matrix.Transform());
	}

	current& Transform(const Transform& hand){
		if(hand.reverse)Reverse();
		Rotate(hand.angle);
		Move(hand.pos);
		return (*this);
	}
	current& Move     (const Point& pos){
		if(pos.x > 0)for(int i=0;i<MATRIX_HEIGHT;i++)this->byte[i] <<=  pos.x;
		if(pos.x < 0)for(int i=0;i<MATRIX_HEIGHT;i++)this->byte[i] >>= -pos.x;
		if(pos.y > 0){
			std::copy(this->byte.rbegin() + pos.y,this->byte.rend(),this->byte.rbegin());
			std::fill(this->byte.rend() - pos.y - 1,this->byte.rend(),std::bitset<MATRIX_WIDTH>());
		}
		if(pos.y < 0){
			std::copy(this->byte.begin()  - pos.y,this->byte.end() ,this->byte.begin());
			std::fill(this->byte.end()  + pos.y + 1,this->byte.end() ,std::bitset<MATRIX_WIDTH>());
		}
		return (*this);
	}
	current& Rotate   (const Constants::ANGLE& angle){
		current tmp = (*this);
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				if(angle == Constants::ANGLE90) tmp[i                   ][MATRIX_HEIGHT - j - 1] = this->byte[j][i];
				if(angle == Constants::ANGLE180)tmp[MATRIX_WIDTH - j - 1][MATRIX_HEIGHT - i - 1] = this->byte[j][i];
				if(angle == Constants::ANGLE270)tmp[MATRIX_WIDTH - i - 1][j                    ] = this->byte[j][i];
			}
	}
		(*this) = tmp;
		return (*this);
	}
	current& Reverse  (){
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				this->byte[j][MATRIX_WIDTH - i - 1] = this->byte[j][i];
			}
		}
		return (*this);
	}
	
	Matrix() = default;
	Matrix(const Base&   base):Base(base){};
	Matrix(const current& mat)  = default;
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>
	Matrix(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix){
		for(int i=0;i<MATRIX_HEIGHT;i++)for(int j=0;j<MATRIX_WIDTH;j++)(*this)[i][j] = matrix[i][j];
	}
	virtual ~Matrix() = default;

	template <size_t WIDTH,size_t HEIGHT> friend std::istream& operator>>(std::istream& ist,Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	template <size_t WIDTH,size_t HEIGHT> friend std::ostream& operator<<(std::ostream& ost,const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	
};

typedef Matrix<FIELD_WIDTH,FIELD_HEIGHT> Field;
typedef Matrix<BLOCK_WIDTH,BLOCK_HEIGHT> Block;

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::ostream& operator<<(std::ostream& ost,const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	ost << std::noboolalpha;
	for(int i=0;i<MATRIX_HEIGHT;i++){
		for(int j=0;j<MATRIX_WIDTH;j++){
			ost << matrix[i][j];
		}
		ost << "\n";
	}
	return ost;
}

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::istream& operator>>(std::istream& ist,Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	for(int i=0;i<MATRIX_HEIGHT;i++){
		std::string str;
		std::getline(ist,str);
		for(int j=0;j < MATRIX_WIDTH;j++){
			matrix[i][j] = (str[j]=='1');
		}
	}
	return ist;
}
/*
template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>
current& Matrix<size_t,size_t>::Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
	return (*this);
}


template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>
current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,Transform trans);

template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix);
template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Point& pos);
template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Transform& hand);

current& Transform(const Transform& hand);
current& Move     (const Point& pos);
current& Rotate   (Constants::ANGLE& angle);
current& Reverse  ();
*/




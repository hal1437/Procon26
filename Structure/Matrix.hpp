
#pragma once
#include <vector>
#include <map>
#include "MultiBit.hpp"
#include "Point.h"
#include "Transform.h"
#include "../Utility/Constants.hpp"
#include "../Utility/CLOCKWISE_FOR.hpp"

#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8

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
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const Transform& trans){
		return Projection(Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>(mat.GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos));
	}

	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix)const{
		return (((*this) & current(matrix)).count() > 0);
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Point& pos)const{
		return Cross(matrix.GetMove(pos));
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Transform& hand)const{
		return Cross(matrix.GetTransform());
	}
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> bool ProjectionTest(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Transform& trans)const{
		Matrix<ARGS_WIDTH,ARGS_HEIGHT>&& mat = matrix.GetTransform(Transform::Transform(Point(0,0),trans.angle,trans.reverse));
		bool adjacent = false;
		for(int i = 0;i < ARGS_HEIGHT;i++){
			for(int j = 0;j < ARGS_WIDTH;j++){
				if(mat[i][j]){
					//overrun
					if(trans.pos.y+i < 0 || trans.pos.x+j < 0 || trans.pos.y+i >= MATRIX_HEIGHT || trans.pos.x+j >= MATRIX_WIDTH){
						return false;
					}
					//crossed
					if(this->byte[trans.pos.y + i][trans.pos.x + j]){
						return false;
					}
					//not Adjacent
					CLOCKWISE_FOR(clockwise){
						Point seach_point = trans.pos + Point(j,i) + clockwise;
						//overrun
						if(seach_point.x < 0 || seach_point.y < 0 || seach_point.y >= MATRIX_HEIGHT || seach_point.x >= MATRIX_WIDTH){
							continue;
						}
						//exist
						if((*this)[seach_point.y][seach_point.x]){
							adjacent = true;
						}
					}
				}
			}
		}
		return adjacent;
	}

	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> std::vector<Transform> GetListLayPossible(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix)const{
		Matrix<ARGS_WIDTH,ARGS_HEIGHT> sample[2][4];
		std::map<current,struct Transform> map;
		for(int i=0;i<4;i++)sample[0][i] = matrix.GetReverse(false) .Rotate(static_cast<Constants::ANGLE>(90*i));
		for(int i=0;i<4;i++)sample[1][i] = matrix.GetReverse(true).Rotate(static_cast<Constants::ANGLE>(90*i));
		
		for(int i=0;i<4;i++){
			std::cout << sample[0][i] << std::endl;
			std::cout << sample[1][i] << std::endl;
		}


		//std::cout << (*this) << std::endl;
		std::vector<class Transform> answer;
		for(int i = 0;i < MATRIX_HEIGHT+ARGS_HEIGHT;i++){
			for(int j = 0;j < MATRIX_WIDTH+ARGS_WIDTH;j++){
				for(int r=0;r<2;r++){
					for(int k=0;k<4;k++){
						if(ProjectionTest(sample[r][k],Transform::Transform(Point(j-ARGS_WIDTH,i-ARGS_HEIGHT),Constants::ANGLE0,false))){
							struct Transform t(Point(j-ARGS_WIDTH,i-ARGS_HEIGHT),static_cast<Constants::ANGLE>(k*90),r);
							map[current(sample[r][k]).Move(t.pos)] = t;
						}
					}
				}
			}
		}
		for(auto x:map)answer.push_back(x.second);
		return answer;
	}

	template<size_t CONVERT_WIDTH,size_t CONVERT_HEIGHT>
	Matrix<CONVERT_WIDTH,CONVERT_HEIGHT> GetTransform(const Transform& trans)const{
		return Matrix<CONVERT_WIDTH,CONVERT_HEIGHT>(this->GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	}

	current GetTransform(const Transform& trans)const{
		return current(*this).Transform(trans);
	}
	current GetMove(const Point& pos)const{
		return current(*this).Move(pos);
	}
	current GetRotate(const Constants::ANGLE& angle)const{
		return current(*this).Rotate(angle);
	}
	current GetReverse(bool _reverse = true)const{
		return current(*this).Reverse(_reverse);
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
			std::fill(this->byte.rend() - pos.y,this->byte.rend(),std::bitset<MATRIX_WIDTH>());
		}
		if(pos.y < 0){
			std::copy(this->byte.begin()  - pos.y,this->byte.end() ,this->byte.begin());
			std::fill(this->byte.end()  + pos.y,this->byte.end() ,std::bitset<MATRIX_WIDTH>());
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
	current& Reverse  (bool _reverse=true){
		if(_reverse){
			current tmp = (*this);
			for(int i=0;i<MATRIX_HEIGHT;i++){
				for(int j=0;j<MATRIX_WIDTH;j++){
					tmp.byte[j][MATRIX_WIDTH - i - 1] = this->byte[j][i];
				}
			}
			(*this) = tmp;
		}
		return (*this);
	}
	
	std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> toBitset()const{
		std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> bits;
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				bits.set(i*MATRIX_WIDTH+j,this->byte[j][i]);
			}
		}
		return bits;
	}

	Matrix() = default;
	Matrix(const Base&   base):Base(base){};
	Matrix(const current& mat)  = default;
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>
	Matrix(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix){
		for(int i=0;i<ARGS_HEIGHT;i++){
			for(int j=0;j<ARGS_WIDTH;j++){
				(*this)[i][j] = matrix[i][j];
			}
		}
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

namespace std {
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class hash<Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>>{
public:
	size_t operator()(const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& mat)const{
		return std::hash<std::bitset<MATRIX_WIDTH * MATRIX_HEIGHT>>()(mat.toBitset());
	}
};
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




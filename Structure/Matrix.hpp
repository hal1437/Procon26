
#pragma once
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <type_traits>
#include <utility>
#include "MultiBit.hpp"
#include "Point.h"
#include "Transform.h"
#include "../Utility/Constants.hpp"
#include "../Utility/CLOCKWISE_FOR.hpp"

#define FIELD_HEIGHT 32
#define FIELD_WIDTH  32
#define BLOCK_HEIGHT 8
#define BLOCK_WIDTH  8

#define DEFINITION_GETTER(NAME)					\
template<class... Args> 						\
inline current Get##NAME(const Args... args)const{	\
	return current(*this).NAME(args...);	\
}
#define MEMBER_TEMPLATE_ARGS \
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT> \
template <size_t ARGS_WIDTH,size_t ARGS_HEIGHT>


template<size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class Matrix:public MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>{
private:
	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> Base;
	typedef Matrix  <MATRIX_WIDTH,MATRIX_HEIGHT> current;

public:

	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat);
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const Transform& trans);
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat);
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT> current& ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const Transform& trans);

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
		if(!trans.isEnable())return false;
		Matrix<ARGS_WIDTH,ARGS_HEIGHT>&& mat = matrix.GetTransform(Transform::Transform(Point(0,0),trans.angle,trans.reverse));
		bool adjacent = false;

		for(int i = 0;i < ARGS_HEIGHT;i++){
			for(int j = 0;j < ARGS_WIDTH;j++){
				if(mat.get(j,i)){
					//overrun
					if(trans.pos.y+i < 0 || trans.pos.x+j < 0 || trans.pos.y+i >= MATRIX_HEIGHT || trans.pos.x+j >= MATRIX_WIDTH){
						return false;
					}
					//crossed
					if((*this)[trans.pos.y + i][trans.pos.x + j]){
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
						if(this->get(seach_point.x,seach_point.y)){
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
		Matrix<MATRIX_WIDTH,MATRIX_HEIGHT> field;
		std::map<Matrix<MATRIX_WIDTH+ARGS_WIDTH,MATRIX_HEIGHT+ARGS_HEIGHT>,struct Transform> map;

		field.Projection(*this);
		Matrix<ARGS_WIDTH,ARGS_HEIGHT> sample_base(matrix);
	
		//std::cout << matrix << std::endl;
		//std::cout << field << std::endl;
		for(int i=0;i<2;i++){
			sample_base.Reverse(i);
			for(int j=0;j<4;j++){
				sample[i][j].Projection(sample_base);
				sample_base.Rotate(Constants::ANGLE90);
			}
		}
	
		std::vector<class Transform> answer;
		for(int i = -8;i < static_cast<int>(MATRIX_HEIGHT);i++){
			for(int j = -8;j < static_cast<int>(MATRIX_WIDTH);j++){
				for(int r=0;r<2;r++){
					for(int k=0;k<4;k++){
						Transform::Transform move_trans(Point(j,i),Constants::ANGLE0,false);
						if(field.ProjectionTest(sample[r][k],move_trans)){
							struct Transform t(Point(j,i),static_cast<Constants::ANGLE>(k*90),r);
							map.insert(std::make_pair(current(field).Projection(sample[r][k],move_trans),t));
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
		if(!trans.isEnable())return (*this);
		return Matrix<CONVERT_WIDTH,CONVERT_HEIGHT>(this->GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	}
	DEFINITION_GETTER(Projection)
	DEFINITION_GETTER(ReverseProjection)
	DEFINITION_GETTER(Transform)
	DEFINITION_GETTER(Move)
	DEFINITION_GETTER(Rotate)
	DEFINITION_GETTER(Reverse)
	
	current& Transform(const Transform& trans){
		if(!trans.isEnable())return (*this);
		if(trans.reverse)Reverse();
		Rotate(trans.angle);
		Move(trans.pos);
		return (*this);
	}
	current& Move     (const Point& pos){
		if(pos.x > 0)(*this) >>=  pos.x;
		if(pos.x < 0)(*this) <<= -pos.x;
		if(pos.y > 0)(*this) >>=  pos.y * MATRIX_WIDTH;
		if(pos.y < 0)(*this) <<= -pos.y * MATRIX_WIDTH;
		return (*this);
	}
	current& Rotate   (const Constants::ANGLE& angle){
		current tmp = (*this);
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				if(angle == Constants::ANGLE90) tmp.set(MATRIX_WIDTH - i - 1 ,j                    ,(*this)[i][j]);
				if(angle == Constants::ANGLE180)tmp.set(MATRIX_WIDTH - j - 1 ,MATRIX_HEIGHT - i - 1,(*this)[i][j]);
				if(angle == Constants::ANGLE270)tmp.set(i                    ,MATRIX_HEIGHT - j - 1,(*this)[i][j]);
			}
		}
		(*this) = tmp;
		return (*this);
	}
	current& Reverse  (bool _reverse=true){
		if(_reverse){
			current tmp(*this);
			for(int i=0;i<MATRIX_HEIGHT;i++){
				for(int j=0;j<MATRIX_WIDTH;j++){
					tmp[i][MATRIX_WIDTH-j-1] = (*this)[i][j];
				}
			}
			(*this) = tmp;
		}
		return (*this);
	}
	current& Normalize(){
		std::set<current> sample;
		

		//rotate reverse
		for(int i=0;i<4;i++){
			for(int j=0;j<2;j++){
				current&& tmp = this->GetTransform(Transform::Transform(Point(0,0),static_cast<Constants::ANGLE>(i*90),j));
				
				Point origin(MATRIX_WIDTH,MATRIX_HEIGHT);
				for(int i=0;i<MATRIX_WIDTH && origin.y == MATRIX_WIDTH;i++){
					for(int j=0;j<MATRIX_HEIGHT && origin.y == MATRIX_WIDTH;j++){
						if(tmp[i][j]){
							origin.y = -i;
						}
					}
				}
				for(int i=0;i<MATRIX_HEIGHT && origin.x == MATRIX_HEIGHT;i++){
					for(int j=0;j<MATRIX_WIDTH && origin.x == MATRIX_HEIGHT;j++){
						if(tmp[j][i]){
							origin.x = -i;
						}
					}
				}
				sample.insert(tmp.Move(origin));
			}
		}

		//move2
		Point origin = Point(MATRIX_WIDTH,MATRIX_HEIGHT);
		for(int i=0;i<MATRIX_WIDTH && origin.y == MATRIX_WIDTH;i++){
			for(int j=0;j<MATRIX_HEIGHT && origin.y == MATRIX_WIDTH;j++){
				if((*sample.begin())[i][j]){
					origin.y = -i;
				}
			}
		}
		for(int i=0;i<MATRIX_HEIGHT && origin.x == MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH && origin.x == MATRIX_HEIGHT;j++){
				if((*sample.begin())[j][i]){
					origin.x = -i;
				}
			}
		}
		//std::cout << origin << std::endl;
		(*this) = sample.begin()->GetMove(origin);
		return (*this);
	}


	current GetReachable()const{
		current answer;
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				if((*this)[i][j])continue;
				CLOCKWISE_FOR(clock){
					Point seach_point = Point(j,i) + clock;
					if(seach_point.x < 0 || seach_point.x>=MATRIX_WIDTH ||
					   seach_point.y < 0 || seach_point.y>=MATRIX_HEIGHT )continue;
					if((*this)[seach_point.y][seach_point.x]){
						answer[i][j] = true;
						break;
					}
				}
			}
		}
		return answer;
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
	
	size_t to_hash()const{
		std::hash< std::bitset<MATRIX_WIDTH * MATRIX_HEIGHT> > hash;
		return hash( toBitset() );
	}

	Matrix() = default;
	Matrix(const Base&   base):Base(base){};
	Matrix(const current& mat)  = default;
	template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>
	Matrix(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix){
		for(int i=0;i<ARGS_HEIGHT;i++){
			for(int j=0;j<ARGS_WIDTH;j++){
				this->set(j,i,matrix.get(j,i));
			}
		}
	}
	virtual ~Matrix() = default;

	template <size_t WIDTH,size_t HEIGHT> friend std::istream& operator>>(std::istream& ist,Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	template <size_t WIDTH,size_t HEIGHT> friend std::ostream& operator<<(std::ostream& ost,const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	
};
//--- Matrix declaration ---//

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

MEMBER_TEMPLATE_ARGS
Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
	(*this) |= current(mat);
	return (*this);
}

MEMBER_TEMPLATE_ARGS
Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const struct Transform& trans){
	if(!trans.isEnable())return (*this);
	(*this) |=  Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>(mat.GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	return	(*this);
}

MEMBER_TEMPLATE_ARGS
Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
	(*this) ^= current(mat);
	return (*this);
}

MEMBER_TEMPLATE_ARGS
Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const struct Transform& trans){
	if(!trans.isEnable())return (*this);
	(*this) ^=  Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>(mat.GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	return	(*this);
}

namespace std{
    template<size_t MATRIX_WIDTH, size_t MATIRX_HEIGHT>
    class hash< Matrix<MATRIX_WIDTH,MATIRX_HEIGHT> >{
        size_t operator()(const Matrix<MATRIX_WIDTH,MATIRX_HEIGHT>& s)const{
            return std::hash< std::bitset<MATRIX_WIDTH*MATIRX_HEIGHT> >()(s.toBitset());
        }
    };
}




#pragma once
#include <vector>
#include <set>
#include <map>
#include <cmath>
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
inline constexpr current Get##NAME(const Args... args)const{	\
	current tmp = (*this);										\
	tmp.NAME(args...);											\
	return tmp;													\
}

#define MEMBER_TEMPLATE_TEMPLATE \
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT> \
template <size_t ARGS_WIDTH,size_t ARGS_HEIGHT>

#define MEMBER_TEMPLATE \
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT> \


#define M_TMP template<size_t ARGS_WIDTH,size_t ARGS_HEIGHT>

template<size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class Matrix:public MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>{
private:
	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> Base;
	typedef Matrix  <MATRIX_WIDTH,MATRIX_HEIGHT> current;

public:

	//Action
	M_TMP constexpr current& Projection       (const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat);
	M_TMP constexpr current& Projection       (const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const Transform& trans);
	M_TMP constexpr current& ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat);
	M_TMP constexpr current& ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const Transform& trans);
	      constexpr current& Transform        (const struct Transform& trnas);
	      constexpr current& Move             (const Point& pos);
	      constexpr current& Rotate           (const Constants::ANGLE& angle);
	      constexpr current& Reverse          (bool  _reverse=true);
	      constexpr current& Normalize        ();
	//Copy-action
	DEFINITION_GETTER(Projection)
	DEFINITION_GETTER(ReverseProjection)
	DEFINITION_GETTER(Transform)
	DEFINITION_GETTER(Move)
	DEFINITION_GETTER(Rotate)
	DEFINITION_GETTER(Reverse)
	DEFINITION_GETTER(Normalize)

	//util
	M_TMP std::vector<struct Transform> GetListLayPossible(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const current& Mask = current(),bool first=false)const;
	M_TMP constexpr bool ProjectionTest(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const struct Transform& trans,const current& Mask = current(),bool first=false)const;
	M_TMP constexpr bool Cross         (const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix                              )const;
	M_TMP constexpr bool Cross         (const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Point& pos             )const;
	M_TMP constexpr bool Cross         (const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const struct Transform& hand )const;
	constexpr current GetReachable()const;

	template<size_t CONVERT_WIDTH,size_t CONVERT_HEIGHT> 
	constexpr Matrix<CONVERT_WIDTH,CONVERT_HEIGHT> GetTransform(const struct Transform& trans)const{
		if(!trans.isEnable())return (*this);
		return Matrix<CONVERT_WIDTH,CONVERT_HEIGHT>(this->GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	}
	

	constexpr std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> toBitset()const{
		std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> bits;
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				bits.set(i*MATRIX_WIDTH+j,(*this)[j][i]);
			}
		}
		return bits;
	}
	
	constexpr size_t to_hash()const{
		static_assert(MATRIX_WIDTH*MATRIX_HEIGHT < 64,"To be oversized matrixsize!" );
		size_t hash=0;
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				hash <<= 1;
				hash |=  (*this)[i][j];
			}
		}
		return hash;
	}

	constexpr Matrix(){};
	constexpr Matrix(const Base&  base):Base(base){};
	constexpr Matrix(const Base&& base):Base(base){}
	constexpr Matrix(size_t hash){
		for(int i=MATRIX_HEIGHT-1;i>=0;i--){
			for(int j=MATRIX_WIDTH-1;j>=0;j--){
				(*this)[i][j] = (hash & 1);
				hash >>= 1;
			}
		}
	};
	template<class T> constexpr Matrix(const std::initializer_list<std::initializer_list<T>> list):Base(list){}
	M_TMP constexpr Matrix(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix){
		for(int i=0;i<ARGS_HEIGHT;i++){
			for(int j=0;j<ARGS_WIDTH;j++){
				this->set(j,i,matrix.get(j,i));
			}
		}
	}

	template <size_t WIDTH,size_t HEIGHT> friend std::istream& operator>>(std::istream& ist,Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	template <size_t WIDTH,size_t HEIGHT> friend std::ostream& operator<<(std::ostream& ost,const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix);
	
};
//--- Matrix declaration ---//

typedef Matrix<FIELD_WIDTH,FIELD_HEIGHT> Field;
typedef Matrix<BLOCK_WIDTH,BLOCK_HEIGHT> Block;


MEMBER_TEMPLATE_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
	(*this) |= current(mat);
	return (*this);
}

MEMBER_TEMPLATE_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Projection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const struct Transform& trans){
	if(!trans.isEnable())return (*this);
	(*this) |=  Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>(mat.GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	return	(*this);
}

MEMBER_TEMPLATE_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat){
	(*this) ^= current(mat);
	return (*this);
}

MEMBER_TEMPLATE_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::ReverseProjection(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& mat,const struct Transform& trans){
	if(!trans.isEnable())return (*this);
	(*this) ^=  Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>(mat.GetReverse(trans.reverse).Rotate(trans.angle)).Move(trans.pos);
	return	(*this);
}

MEMBER_TEMPLATE_TEMPLATE
constexpr bool Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix)const{
	return (((*this) & current(matrix)).count() > 0);
}
MEMBER_TEMPLATE_TEMPLATE
constexpr bool Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const Point& pos)const{
	return Cross(matrix.GetMove(pos));
}
MEMBER_TEMPLATE_TEMPLATE
constexpr bool Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Cross(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const struct Transform& hand)const{
	return Cross(matrix.GetTransform());
}

MEMBER_TEMPLATE_TEMPLATE
constexpr bool Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::ProjectionTest(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const struct Transform& trans,const current& Mask,bool first)const{
	if(!trans.isEnable())return false;
	Matrix<ARGS_WIDTH,ARGS_HEIGHT>&& mat = matrix.GetTransform(Transform::Transform(Point(0,0),trans.angle,trans.reverse));
	bool adjacent = false;

	for(int i = 0;i < ARGS_HEIGHT;i++){
		for(int j = 0;j < ARGS_WIDTH;j++){
			if(mat.get(j,i)){
				if(trans.pos.y+i < 0 || trans.pos.x+j < 0 || trans.pos.y+i >= MATRIX_HEIGHT || trans.pos.x+j >= MATRIX_WIDTH)return false;
				if( ((*this) | Mask)[trans.pos.y + i][trans.pos.x + j])return false;
				
				if(!first){
					CLOCKWISE_FOR(clockwise){
						Point seach_point = trans.pos + Point(j,i) + clockwise;
						if(seach_point.x < 0 || seach_point.y < 0 || seach_point.y >= MATRIX_HEIGHT || seach_point.x >= MATRIX_WIDTH)continue;
						if(this->get(seach_point.x,seach_point.y))adjacent = true;
					}
				}
			}
		}
	}
	return (first || adjacent);
}

MEMBER_TEMPLATE_TEMPLATE
std::vector<Transform> Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::GetListLayPossible(const Matrix<ARGS_WIDTH,ARGS_HEIGHT>& matrix,const current& Mask,bool first)const{
	Matrix<ARGS_WIDTH,ARGS_HEIGHT> sample[2][4];
	const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& field = *this;
	const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>&& project = (*this|Mask);
	std::map<Matrix<MATRIX_WIDTH+ARGS_WIDTH,MATRIX_HEIGHT+ARGS_HEIGHT>,struct Transform> map;
	Matrix<ARGS_WIDTH,ARGS_HEIGHT> sample_base(matrix);
	int triming[4];

	std::fill(triming,triming+4,-1);

	for(int i=0;i<MATRIX_HEIGHT;i++){
		for(int j=0;j<MATRIX_WIDTH;j++){
			Point mirror(MATRIX_WIDTH-j-1,MATRIX_HEIGHT-i-1);
			if(triming[Constants::DIRECTION::UP] == -1 && project[i][j]==false){
				triming[Constants::DIRECTION::UP] = i;
			}
			if(triming[Constants::DIRECTION::LEFT] == -1 && project[j][i]==false){
				triming[Constants::DIRECTION::LEFT] = i;
			}
			if(triming[Constants::DIRECTION::DOWN] == -1 && project[mirror.y][mirror.x]==false){
				triming[Constants::DIRECTION::DOWN] = MATRIX_HEIGHT - mirror.y -1;
			}
			if(triming[Constants::DIRECTION::RIGHT] == -1 && project[mirror.x][mirror.y]==false){
				triming[Constants::DIRECTION::RIGHT] = MATRIX_HEIGHT - mirror.y - 1;
			}

			if(triming[Constants::DIRECTION::UP]    != -1 &&
			   triming[Constants::DIRECTION::RIGHT] != -1 &&
			   triming[Constants::DIRECTION::DOWN]  != -1 &&
			   triming[Constants::DIRECTION::LEFT]  != -1){
				break;
			}
		}
	}
	for(int i=0;i<4;i++){
		std::cout << triming[i] << std::endl;
	}

	for(int i=0;i<2;i++){
		sample_base.Reverse(i);
		for(int j=0;j<4;j++){
			sample[i][j].Projection(sample_base);
			sample_base.Rotate(Constants::ANGLE90);
		}
	}

	std::vector<class Transform> answer;
	for(int i = -8 + triming[Constants::DIRECTION::UP];i < static_cast<int>(MATRIX_HEIGHT) - triming[Constants::DIRECTION::DOWN];i++){
		for(int j = -8 + triming[Constants::DIRECTION::LEFT];j < static_cast<int>(MATRIX_WIDTH) - triming[Constants::DIRECTION::RIGHT];j++){
			for(int r=0;r<2;r++){
				for(int k=0;k<4;k++){
					Transform::Transform move_trans(Point(j,i),Constants::ANGLE0,false);
					if((first && (Mask.ProjectionTest(sample[r][k],move_trans,current(),true))) || field.ProjectionTest(sample[r][k],move_trans,Mask)){
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

MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Transform(const struct Transform& trans){
	if(!trans.isEnable())return (*this);
	if(trans.reverse)Reverse();
	Rotate(trans.angle);
	Move(trans.pos);
	return (*this);
}
MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Move     (const Point& pos){
	if(pos.y > 0)(*this) >>=  pos.y * Base::ARRAY_MATRIX_WIDTH*8;
	if(pos.y < 0)(*this) <<= -pos.y * Base::ARRAY_MATRIX_WIDTH*8;
	if(pos.x > 0)(*this) >>=  pos.x;
	if(pos.x < 0)(*this) <<= -pos.x;
	return (*this);
}
MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Rotate   (const Constants::ANGLE& angle){
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
MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Reverse  (bool _reverse){
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
MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::Normalize(){
	//std::set<current> sample;
	current sample[8];
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
			sample[j*4 + i] = tmp.GetMove(origin);
		}
	}
	current& most = sample[0];
	for(int i=0;i<8;i++){
		if(most < sample[i])most = sample[i];
	}

	//move2
	Point origin = Point(MATRIX_WIDTH,MATRIX_HEIGHT);
	for(int i=0;i<MATRIX_WIDTH && origin.y == MATRIX_WIDTH;i++){
		for(int j=0;j<MATRIX_HEIGHT && origin.y == MATRIX_WIDTH;j++){
			if(most[i][j]){
				origin.y = -i;
			}
		}
	}
	for(int i=0;i<MATRIX_HEIGHT && origin.x == MATRIX_HEIGHT;i++){
		for(int j=0;j<MATRIX_WIDTH && origin.x == MATRIX_HEIGHT;j++){
			if(most[j][i]){
				origin.x = -i;
			}
		}
	}
	(*this) = most.GetMove(origin);
	return (*this);
}

MEMBER_TEMPLATE
constexpr Matrix<MATRIX_WIDTH,MATRIX_HEIGHT> Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>::GetReachable()const{
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


template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::ostream& operator<<(std::ostream& ost,const Matrix<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	ost << std::noboolalpha;
	for(int i=0;i<MATRIX_HEIGHT;i++){
		for(int j=0;j<MATRIX_WIDTH;j++){
#ifdef MATRIX_WIDEOUT
			if(matrix[i][j]){
				ost << "■";
			}else{
				ost << "□";
			}
#else
			ost << matrix[i][j];
#endif
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

namespace std{
	template<size_t MATRIX_WIDTH, size_t MATIRX_HEIGHT>
	struct hash< Matrix<MATRIX_WIDTH,MATIRX_HEIGHT> >{
		size_t operator()(const Matrix<MATRIX_WIDTH,MATIRX_HEIGHT>& s)const{
			return s.GetNormalize().to_hash();
		}
	};
}








#pragma once
#include <vector>
#include "Matrix.hpp"
#include "Transform.h"

template<size_t MATIX_WIDHT,size_t MATRIX_HEIGHT>
struct BlockState{
	Matrix<MATIX_WIDHT,MATRIX_HEIGHT> matrix;
	Transform trans;
};

template<size_t MATIX_WIDHT,size_t MATRIX_HEIGHT>
class Layer:public std::vector<BlockState<MATIX_WIDHT,MATRIX_HEIGHT>>{
private:
	typedef BlockState<MATIX_WIDHT,MATRIX_HEIGHT> State;
	typedef std::vector<BlockState<MATIX_WIDHT,MATRIX_HEIGHT>> Base;
public:

    void returnTheHand(int index,Field& field){
        for(int i=0;i<this->size()-index;i++){
            field.ReverseProjection(this->back().trans, this->back().matrix);
            this->pop_back();
        }
    }

    Layer(){};
	virtual ~Layer(){};
};

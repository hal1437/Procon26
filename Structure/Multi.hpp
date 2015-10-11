#pragma once

#include <vector>
#include <iostream>
#include <type_traits>
#include <initializer_list>
#include "Point.h"

template<class T,std::size_t... Dims>
class Multi{
public:
	typedef T  value_type;
	typedef T  child;
	typedef T* iterator;
	typedef const T* const_iterator;
	T _value;
public:

	iterator begin(){
		return &_value;
	}
	iterator end(){
		return &(_value+1);
	}
	const_iterator begin()const{
		return &_value;
	}
	const_iterator end()const{
		return &(_value+1);
	}

	value_type& operator=(const value_type& rhs){
		_value = rhs;
		return _value;
	}
	value_type& operator=(const Multi<T>& rhs){
		_value = rhs._value;
		return _value;
	}
	
	Multi() = default;
	Multi(T init):_value(init){}

	operator value_type&(){
		return _value;
	}
	operator const value_type&()const{
		return _value;
	}
};
template<class T,std::size_t First,std::size_t... Dims>
class Multi<T,First,Dims...>{
public:
	typedef T  value_type;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef Multi<T,Dims...> child;	
	
public:
	child _array[First];

public:

	iterator begin(){
		return _array[0].begin();
	}
	iterator end(){
		return _array[First].begin();
	}
	const_iterator begin()const{
		return _array[0].begin();
	}
	const_iterator end()const{
		return _array[First].begin();
	}
	child& operator[](const std::size_t& index){
		return _array[index];
	}
	const child& operator[](const std::size_t& index)const {
		return _array[index];
	}
	typename child::child& operator[](const Point& index){
		return _array[index.y][index.x];
	}
	const typename child::child& operator[](const Point& index)const {
		return _array[index.y][index.x];
	}

	Multi(){};
	Multi(std::initializer_list<child> init){
		std::copy(init.begin(),init.end(),_array);
	}

};

template<class T,std::size_t Dims>
class MultiVector{
public:
	typedef T value_type;
	typedef MultiVector<T,Dims-1> child;
public:
	std::vector<child> _array;

	template<class... Args>
	void resize(std::size_t first ,Args... body){
		_array.resize(first);
		for(child& c:_array){
			c.resize(body...);
		}
	};

	child& operator[](const std::size_t& index){
		return _array[index];
	}

};

//specialized
template<class T>
class MultiVector<T,0>{
public:
	typedef T value_type;
	typedef T child;
public:
	T _value;
	
	void resize(){
	};

	value_type& operator=(const value_type& rhs){	
		_value = rhs;
		return _value;
	}
	operator value_type&(){
		return _value;
	}
};

template<class T,size_t X,size_t Y>
std::ostream& operator<<(std::ostream& out,const Multi<T,X,Y> field){
	for(int i=0;i<Y;i++){
		for(int j=0;j<X;j++){
			out << field[i][j];
		}
		out << "\r\n";
	}
	return out;
}


template<class T,size_t X,size_t Y>
std::istream& operator>>(std::istream& in,Multi<T,X,Y>& field){
	for(int i=0;i<Y;i++){
		std::string str;
		std::getline(in,str);
		for(int j=0;j<X;j++){
			field[i][j] = static_cast<T>(str[j]-'0');
		}
	}
	return in;
}


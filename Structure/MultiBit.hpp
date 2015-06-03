
#pragma once
#include <bitset>
#include <array>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <numeric> 

#define OVERRIDE_OPERATOR(OPERATOR)																					\
MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> operator OPERATOR (const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& value)const{	\
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> cp;																		\
	for(int i=0;i<MATRIX_HEIGHT;i++)this->byte[i] OPERATOR##= value.byte[i];							\
	return cp;																										\
}

#define OVERRIDE_OPERATOR_REF(OPERATOR) 								\
MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator OPERATOR (const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& value){	\
	for(int i=0;i<MATRIX_HEIGHT;i++)this->byte[i] OPERATOR value.byte[i];										\
	return (*this);																								\
}																		


template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class MultiBit{
	//00000000 00000000 00000000 00000000
public:
	friend class FirstProxy;
	friend class SecondProxy;

	class SecondProxy;
	class FirstProxy{
		MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>* ref;
		const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>* c_ref;
		int y;
		friend class SecondProxy;
	public:
		SecondProxy operator[](size_t index){
			return SecondProxy(this,index);
		}
		SecondProxy operator[](size_t index)const{
			return SecondProxy(this,index);
		}
		FirstProxy(      MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>*const Ref,size_t index):ref(Ref),c_ref(Ref),y(index){}
		FirstProxy(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>*const Ref,size_t index):         c_ref(Ref),y(index){}
	};
	class SecondProxy{
		FirstProxy* f_proxy;
		const FirstProxy* c_f_proxy;
		int x;
	public:
		MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator=(bool value){
			f_proxy->ref->set(x,f_proxy->y,value);
			return (*f_proxy->ref);
		}
		operator bool()const{
			return c_f_proxy->c_ref->get(x,c_f_proxy->y);
		}
		SecondProxy& operator=(const SecondProxy& rhs){
			f_proxy->ref->set(x,f_proxy->y,static_cast<bool>(rhs));
			return (*this);
		}
		SecondProxy()=default;
		SecondProxy(      FirstProxy* Ref,size_t index):f_proxy(Ref),c_f_proxy(Ref),x(index){}
		SecondProxy(const FirstProxy* Ref,size_t index):             c_f_proxy(Ref),x(index){}
	};

protected:
	const int MATRIX_SIZE = MATRIX_WIDTH * MATRIX_HEIGHT;
	const int BYTE_SIZE = 8;
	std::array<std::bitset<MATRIX_WIDTH>,MATRIX_HEIGHT> byte;
		
public:

	bool get(size_t x,size_t y)const{
		return byte[y][x];
	}
	void set(size_t x,size_t y,bool value = true){
		byte[y].set(x,value);
	}
	size_t count()const{
		size_t size;
		for(const std::bitset<MATRIX_WIDTH>& bits : byte)size += bits.count();
		return size;
	}

	FirstProxy operator[](size_t index){
		return FirstProxy(this,index);
	}
	FirstProxy operator[](size_t index)const{
		return FirstProxy(this,index);
	}
	
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator=(MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& origin){
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				byte[i].set(j,origin.byte[i][j]);
			}
		}
		return (*this);
	}
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator=(MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&& origin){
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				byte[i].set(j,origin.byte[i][j]);
			}
		}
		return (*this);
	}

	bool operator==(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs)const{
		return  std::equal(byte,byte+MATRIX_HEIGHT);
	}
	bool operator!=(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs)const{
		return !std::equal(byte,byte+MATRIX_HEIGHT);
	}

	OVERRIDE_OPERATOR(&)
	OVERRIDE_OPERATOR(|)
	OVERRIDE_OPERATOR(^)
	OVERRIDE_OPERATOR(<<)
	OVERRIDE_OPERATOR(>>)
	OVERRIDE_OPERATOR_REF(&=)
	OVERRIDE_OPERATOR_REF(|=)
	OVERRIDE_OPERATOR_REF(^=)
	OVERRIDE_OPERATOR_REF(<<=)
	OVERRIDE_OPERATOR_REF(>>=)

	MultiBit(){}
	MultiBit(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&  origin){
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				byte[i][j] = origin[i][j];
			}
		}
	}
	MultiBit(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&& origin){
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				byte[i][j] = origin[i][j];
			}
		}
	}
	template<class T>
	MultiBit(std::initializer_list<std::initializer_list<T>> init){
		int i=0,j=0;
		for(auto& it1 : init){
			for(auto& it2 :it1){
				byte.set(i * MATRIX_WIDTH + j,it2);
				j++;
			}
			j=0;
			i++;
		}
	}
	virtual  ~MultiBit(){
	}

	template <size_t WIDTH,size_t HEIGHT>
	friend std::ostream& operator<<(std::ostream& ost,const MultiBit<WIDTH,HEIGHT>& matrix);
	template <size_t WIDTH,size_t HEIGHT>
	friend std::istream& operator>>(std::istream& ist,MultiBit<WIDTH,HEIGHT>& matrix);
};

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::ostream& operator<<(std::ostream& ost,const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	for(int i=0;i<MATRIX_HEIGHT;i++){
		for(int j=0;j<MATRIX_WIDTH;j++){
			ost << matrix.byte[i][j];
		}
		ost << "\n";
	}
	return ost;
}

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::istream& operator>>(std::istream& ist,MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	for(int i=0;i<MATRIX_HEIGHT;i++){
		std::string str;
		std::getline(ist,str);
		for(int j=0;j < MATRIX_WIDTH;j++){
			matrix.byte[i][j] = (str[j]=='1');
		}
	}
	return ist;
}

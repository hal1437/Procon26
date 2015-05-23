
#pragma once
#include <bitset>
#include <initializer_list>
#include <iostream>
#include <algorithm>

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
	static const int BYTE_SIZE = 8;
	static const int MATRIX_SIZE = MATRIX_WIDTH * MATRIX_HEIGHT;
	unsigned char byte[MATRIX_SIZE / BYTE_SIZE]={};
public:

	bool get(size_t x,size_t y)const{
		return byte[(y*MATRIX_WIDTH / BYTE_SIZE) + (x / BYTE_SIZE)] & 1 << (BYTE_SIZE - (x % BYTE_SIZE) - 1);
	}
	void set(size_t x,size_t y,bool value = true){
		bool selecting = get(x,y);
		if(selecting && !value){
			byte[(y * MATRIX_WIDTH / BYTE_SIZE) + (x / BYTE_SIZE)] -= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
		if(!selecting && value){
			byte[(y * MATRIX_WIDTH / BYTE_SIZE) + (x / BYTE_SIZE)] |= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
	}
	size_t count(){
		return std::bitset<MATRIX_SIZE>(byte).count();
	}

	FirstProxy operator[](size_t index){
		return FirstProxy(this,index);
	}
	FirstProxy operator[](size_t index)const{
		return FirstProxy(this,index);
	}

	MultiBit(){
		static_assert(MATRIX_WIDTH % BYTE_SIZE == 0,"[static_assert:MultiBit] MATRIX_WIDTH cannot be divided.");
	}
	template<class T>
	MultiBit(std::initializer_list<std::initializer_list<T>> init){
		static_assert(MATRIX_WIDTH % BYTE_SIZE == 0,"[static_assert:MultiBit] MATRIX_WIDTH cannot be divided.");
		int i=0,j=0;
		for(auto& it1 : init){
			for(auto& it2 :it1){
				set(j,i,it2);
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
		for(int j=0;j<MATRIX_WIDTH / matrix.BYTE_SIZE;j++){
			ost << std::bitset<8>(matrix.byte[i * MATRIX_WIDTH / matrix.BYTE_SIZE + j]);
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
			//std::cout << (str[j]=='1');
			matrix.set(j,i,str[j]=='1');
		}
		//std::cout << "\n";
	}
	return ist;
}

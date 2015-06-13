
#pragma once
#include <bitset>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <cmath>

#define PP_CEIL(x) ((x-static_cast<int>(x) > 0) ? x+1 : x)

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


/*
 *       Multibit<20,10>
 *
 *　　　ARRAY_MATRIX_WIDTH
 *├────────────┤
 *│   　MATRIX_WIDTH     　│
 *├──────────┼─┤
 * 00000000 00000000 00000000┬
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000│MATRIX_WIDTH
 * 00000000 00000000 00000000│ARRAY_MATRIX_WIDTH 
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000│
 * 00000000 00000000 00000000┴
 *　　　　　　　　　├───┤
 *  　　　　　　　　BYTE_SIZE:8
*/

protected:
	static constexpr int BYTE_SIZE           = 8;
	static constexpr int ARRAY_MATRIX_WIDTH  = PP_CEIL(MATRIX_WIDTH / 8.0f);				//0~4
	static constexpr int ARRAY_MATRIX_HEIGHT = MATRIX_HEIGHT;								//0~32
	static constexpr int ARRAY_MATRIX_SIZE   = ARRAY_MATRIX_WIDTH * ARRAY_MATRIX_HEIGHT;	//
	static constexpr int MATRIX_SIZE         = MATRIX_WIDTH * MATRIX_HEIGHT;
	unsigned char byte[ARRAY_MATRIX_SIZE]={};

	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> current;
public:

	static constexpr int WIDTH  = MATRIX_WIDTH;
	static constexpr int HEIGHT = MATRIX_HEIGHT;

	bool get(size_t index)const{
		return byte[index / BYTE_SIZE] & 1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1);
	}
	bool get(size_t x,size_t y)const{
		return byte[y*ARRAY_MATRIX_WIDTH + x /BYTE_SIZE] & 1 << (BYTE_SIZE - (x % BYTE_SIZE) - 1);
	}
	void set(size_t index,bool value){
		bool selecting = get(index);
		if(selecting && !value){
			byte[index / BYTE_SIZE] -= (1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1));
		}
		if(!selecting && value){
			byte[index / BYTE_SIZE] |= (1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1));
		}
	}
	void set(size_t x,size_t y,bool value){
		bool selecting = get(x,y);
		if(selecting && !value){
			byte[y * ARRAY_MATRIX_WIDTH + (x / BYTE_SIZE)] -= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
		if(!selecting && value){
			byte[y * ARRAY_MATRIX_WIDTH + (x / BYTE_SIZE)] |= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
	}
	size_t count()const{
		size_t size=0;
		for(int i=0;i < MATRIX_SIZE / BYTE_SIZE;i++){
			char s = byte[i];
			for(int j=0;j<BYTE_SIZE;j++){
				if(s & 1)size++;
				s >>= 1;
			}
		}
		return size;
	}

	FirstProxy operator[](size_t index){
		return FirstProxy(this,index);
	}
	FirstProxy operator[](size_t index)const{
		return FirstProxy(this,index);
	}
	bool operator==(const current& rhs)const{
		return std::equal(byte,byte+ARRAY_MATRIX_SIZE,rhs.byte);
	}
	bool operator!=(const current& rhs)const{
		return !std::equal(byte,byte+ARRAY_MATRIX_SIZE,rhs.byte);
	}
	bool operator<(const current rhs)const{
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++){
			if(this->byte[i] < rhs.byte[i])return true;
			if(this->byte[i] > rhs.byte[i])return false;
		}
		return false;
	}
	current& operator~(){
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)byte[i] = ~byte[i];
		return (*this);
	}
	current operator&(const current& rhs)const{
		current answer;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i] = (this->byte[i] & rhs.byte[i]);
		return answer;
	}
	current operator|(const current& rhs)const{
		current answer;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i] = (this->byte[i] | rhs.byte[i]);
		return answer;
	}
	current operator^(const current& rhs)const{
		current answer;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i] = (this->byte[i] ^ rhs.byte[i]);
		return answer;
	}
	current& operator&=(const current& rhs){
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)byte[i] &= rhs.byte[i];
		return (*this);
	}
	current& operator|=(const current& rhs){
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)byte[i] |= rhs.byte[i];
		return (*this);
	}
	current& operator^=(const current& rhs){
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)byte[i] ^= rhs.byte[i];
		return (*this);
	}
	current&& operator<<(size_t value)const{
		current answer;
		for(int i=ARRAY_MATRIX_SIZE-value;i>0;i--)answer.set(i,this->get(i+value));
		return answer;
	}
	current&& operator>>(size_t value)const{
		current answer;
		for(int i=value;i<ARRAY_MATRIX_SIZE;i++)answer.set(i,get(i-value));
		return answer;
	}
	current& operator<<=(size_t value){
		for(int i=0;i<MATRIX_SIZE-value;i++)this->set(i,this->get(i+value));
		for(int i=0;i<value;i++)set(i,0);
		return (*this);
	}
	current& operator>>=(size_t value){
		for(int i=MATRIX_SIZE;i>=value;i--)set(i,get(i-value));
		for(int i=value;i>=0;i--)set(i,0);
		return (*this);
	}



	MultiBit(){
	}
	MultiBit(const current& origin){
		std::copy(origin.byte,origin.byte+ARRAY_MATRIX_SIZE,byte);
	}
	template<class T>
	MultiBit(std::initializer_list<std::initializer_list<T>> init){
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

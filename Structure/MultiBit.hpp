
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

	constexpr bool get(size_t index)const{
		return byte[index / BYTE_SIZE] & 1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1);
	}
	constexpr bool get(size_t x,size_t y)const{
		return byte[y*ARRAY_MATRIX_WIDTH + x /BYTE_SIZE] & 1 << (BYTE_SIZE - (x % BYTE_SIZE) - 1);
	}
	constexpr void set(size_t index,bool value){
		bool selecting = get(index);
		if(selecting && !value){
			byte[index / BYTE_SIZE] -= (1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1));
		}
		if(!selecting && value){
			byte[index / BYTE_SIZE] |= (1UL << (BYTE_SIZE - (index % BYTE_SIZE) - 1));
		}
	}
	constexpr void set(size_t x,size_t y,bool value){
		bool selecting = get(x,y);
		if(selecting && !value){
			byte[y * ARRAY_MATRIX_WIDTH + (x / BYTE_SIZE)] -= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
		if(!selecting && value){
			byte[y * ARRAY_MATRIX_WIDTH + (x / BYTE_SIZE)] |= (1UL << (BYTE_SIZE - (x % BYTE_SIZE) - 1));
		}
	}
	constexpr size_t count()const{
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
	/*
	unsigned long to_ulong()const{
		unsigned long value;
	}*/

	FirstProxy operator[](size_t index){
		return FirstProxy(this,index);
	}
	constexpr FirstProxy operator[](size_t index)const{
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
	constexpr current operator~()const{
		current tmp;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)tmp.byte[i] = ~tmp.byte[i];
		return tmp;
	}
	constexpr current operator&(const current& rhs)const{
		current answer;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i] = (this->byte[i] & rhs.byte[i]);
		return answer;
	}
	constexpr current operator|(const current& rhs)const{
		current answer;
		for(int i=0;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i] = (this->byte[i] | rhs.byte[i]);
		return answer;
	}
	constexpr current operator^(const current& rhs)const{
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
	constexpr current operator<<(size_t value)const{
		current answer(*this);
		if(value >= BYTE_SIZE){
			for(int i = value/BYTE_SIZE;i<ARRAY_MATRIX_SIZE;i++)answer.byte[i-value/BYTE_SIZE] = answer.byte[i];
			for(int i = ARRAY_MATRIX_SIZE-1;i>=ARRAY_MATRIX_SIZE - 1 - value/BYTE_SIZE;i--)answer.byte[i] = 0;
			value %= 8;
		}
		if(!value)return answer;

		char hi=0, lo=0;
		char mask = (char)(0xFF << (BYTE_SIZE - value));
		for(int  i = ARRAY_MATRIX_SIZE-1 ; i >= 0 ; i-- ){
			hi = (answer.byte[i + 1] & mask) << (BYTE_SIZE - value);
			lo = (answer.byte[i + 0] << value);
			if ( i != 0 )answer.byte[i] = (hi | lo);
			else         answer.byte[i] = (lo);
		}	
		return answer;
	}
	constexpr current operator>>(size_t value)const{
		current answer(*this);
		if(value >= BYTE_SIZE){
			for(int i = ARRAY_MATRIX_SIZE - 1 - value/BYTE_SIZE;i >= 0 ;i--)answer.byte[i+value/BYTE_SIZE] = answer.byte[i];
			for(int i = 0;i<value/BYTE_SIZE;i++)answer.byte[i] = 0;
			value %= 8;
		}
		if(!value)return answer;
		char hi=0, lo=0;
		char mask = (char)(0xFF >> (BYTE_SIZE - value));
		for(int  i = ARRAY_MATRIX_SIZE-1 ; i >= 0 ; i-- ){
			if(i!=0)hi = (answer.byte[i - 1] & mask) << (BYTE_SIZE - value);
			lo = (answer.byte[i - 0] >> value);
			if ( i != 0 )answer.byte[i] = (hi | lo);
			else         answer.byte[i] = (lo);
		}
		return answer;
	}
	current& operator<<=(size_t value){
		if(value >= BYTE_SIZE){
			for(int i = value/BYTE_SIZE;i<ARRAY_MATRIX_SIZE;i++)byte[i-value/BYTE_SIZE] = byte[i];
			for(int i = ARRAY_MATRIX_SIZE-1;i>value/BYTE_SIZE;i--)byte[i] = 0;
			value %= 8;
		}
		if(!value)return (*this);

		char hi, lo;
		char mask = (char)(0xFF << (BYTE_SIZE - value));
		for(int  i = ARRAY_MATRIX_SIZE-1 ; i >= 0 ; i-- ){
			hi = (byte[i + 1] & mask) << (BYTE_SIZE - value);
			lo = (byte[i + 0] << value);
			if ( i != 0 )byte[i] = (hi | lo);
			else         byte[i] = (lo);
		}	
		
		return (*this);
	}
	current& operator>>=(size_t value){
		if(value >= BYTE_SIZE){
			for(int i = ARRAY_MATRIX_SIZE - 1 - value/BYTE_SIZE;i >= 0 ;i--)byte[i+value/BYTE_SIZE] = byte[i];
			for(int i = 0;i<value/BYTE_SIZE;i++)byte[i] = 0;
			value %= 8;
		}
		if(!value)return (*this);
		char hi, lo;
		char mask = (char)(0xFF >> (BYTE_SIZE - value));
		for(int  i = ARRAY_MATRIX_SIZE-1 ; i >= 0 ; i-- ){
			if(i!=0)hi = (byte[i - 1] & mask) << (BYTE_SIZE - value);
			lo = (byte[i - 0] >> value);
			if ( i != 0 )byte[i] = (hi | lo);
			else         byte[i] = (lo);
		}
		return (*this);
	}



	constexpr MultiBit(){
	}
	template<class T>
	constexpr MultiBit(std::initializer_list<std::initializer_list<T>> init){
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
	//virtual  ~MultiBit(){
	//}

	template <size_t WIDTH,size_t HEIGHT>
	friend std::ostream& operator<<(std::ostream& ost,const MultiBit<WIDTH,HEIGHT>& matrix);
	template <size_t WIDTH,size_t HEIGHT>
	friend std::istream& operator>>(std::istream& ist,MultiBit<WIDTH,HEIGHT>& matrix);
};

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::ostream& operator<<(std::ostream& ost,const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	for(int i=0;i < MATRIX_HEIGHT;i++){
		for(int j=0;j < MATRIX_WIDTH;j++){
			ost << matrix[i][j];
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

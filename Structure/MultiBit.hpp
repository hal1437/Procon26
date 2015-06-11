
#pragma once
#include <bitset>
#include <array>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <numeric> 

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class MultiBit:protected std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT>{
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
	std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> byte;
	
	typedef MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> current;
	typedef std::bitset<MATRIX_WIDTH*MATRIX_HEIGHT> Base;

public:

	friend std::ostream& operator<<(std::ostream& ost,const current& matrix);
	friend std::istream& operator>>(std::istream& ist,const current& matrix);
	current operator&(const current& rhs)const{current mat(*this);mat &= rhs;return mat;}
	current operator|(const current& rhs)const{current mat(*this);mat |= rhs;return mat;}
	current operator^(const current& rhs)const{current mat(*this);mat ^= rhs;return mat;}
	Base& operator&=(const current& i) { return Base::operator&=(i); }
	Base& operator|=(const current& i) { return Base::operator|=(i); }
	Base& operator^=(const current& i) { return Base::operator^=(i); }
	using Base::count;

	bool get(size_t x,size_t y)const{
		return byte[y*MATRIX_WIDTH + x];
	}
	void set(size_t x,size_t y,bool value = true){
		byte.set(y*MATRIX_WIDTH + x,value);
	}



	bool operator==(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs)const{
		for(int i=0;i<MATRIX_SIZE;i++)if(!(this->byte[i] & rhs.byte[i]))return false;
		return true;
	}
	bool operator!=(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs)const{
		for(int i=0;i<MATRIX_SIZE;i++)if(this->byte[i] & rhs.byte[i])return false;
		return true;
	}

	bool operator<(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs)const{
		for(int i=0;i<MATRIX_HEIGHT;i++){
			for(int j=0;j<MATRIX_WIDTH;j++){
				if( rhs[i][j] && !(*this)[i][j])return true;
				if(!rhs[i][j] &&  (*this)[i][j])return false;
			}
		}
		return false;
	}
	FirstProxy operator[](size_t index){
		return FirstProxy(this,index);
	}
	FirstProxy operator[](size_t index)const{
		return FirstProxy(this,index);
	}
	
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator=(MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& origin){
		for(int i=0;i<MATRIX_SIZE;i++){
			byte.set(i,origin.byte[i]);
		}
		return (*this);
	}
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& operator=(MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&& origin){
		for(int i=0;i<MATRIX_SIZE;i++){
			byte.set(i,origin.byte[i]);
		}
		return (*this);
	}

	MultiBit(){}
	MultiBit(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&  origin){
		for(int i=0;i<MATRIX_HEIGHT*MATRIX_WIDTH;i++){
			(*this)[i] = origin[i];
		}
	}
	MultiBit(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>&& origin){
		for(int i=0;i<MATRIX_HEIGHT*MATRIX_WIDTH;i++){
			(*this)[i] = origin[i];
		}
	}
	template<class T>
	MultiBit(std::initializer_list<std::initializer_list<T>> init){
		int i=0,j=0;
		for(auto& it1 : init){
			for(auto& it2 :it1){
				Base::set(i * MATRIX_WIDTH + j,it2);
				j++;
			}
			j=0;
			i++;
		}
	}
	virtual  ~MultiBit(){
	}

};

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
std::ostream& operator<<(std::ostream& ost,const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
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
std::istream& operator>>(std::istream& ist,MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& matrix){
	for(int i=0;i<MATRIX_HEIGHT;i++){
		std::string str;
		std::getline(ist,str);
		for(int j=0;j < MATRIX_WIDTH;j++){
			matrix[i][j] = (str[j]=='1');
		}
	}
	return ist;
}

template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> operator&(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& lhs,
											   const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs){
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> mat;
	mat &= lhs &= rhs;
	return mat;
}
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> operator|(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& lhs,
											   const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs){
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> mat;
	mat |= lhs |= rhs;
	return mat;
}
template <size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> operator^(const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& lhs,
											   const MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT>& rhs){
	MultiBit<MATRIX_WIDTH,MATRIX_HEIGHT> mat;
	mat ^= lhs ^= rhs;
	return mat;
} 


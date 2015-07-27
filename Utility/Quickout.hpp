
#pragma once

#include <iostream>
#define Qo Quickout

void Quickout(){
#ifndef _QUICKOUT_DISABLE
	std::cout << std::endl;
#endif
}

template<class X,class... XS>
void Quickout(const X& head,const XS&... body){
#ifndef _QUICKOUT_DISABLE
	std::cout << head << " ";
	Quickout(body...);
#endif
}



#pragma once

//This preprocesser will create a Get function and Set function.
#define SYNTHESIZE(TYPE,NAME,FUNC)                           \
protected:                                                   \
	TYPE NAME;                                               \
public:                                                      \
	virtual void set##FUNC(TYPE NAME){this->##NAME = NAME;}  \
	virtual TYPE get##FUNC(void)const{return this->##NAME;}  \


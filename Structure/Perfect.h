
#pragma once

template<class ArgType...>
class Perfect{
public:
	virtual bool Execution(const ArgType...& arg)=0;
};


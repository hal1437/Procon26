
#pragma once

template<class ReturnType,class ArgType>
class Heuristics{
public:
	virtual ReturnType Execution(const ArgType& arg)=0;
};


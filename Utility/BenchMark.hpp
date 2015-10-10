
#pragma once 
#include <functional>
#include <chrono>


template<size_t COUNT>
class BenchMark{
public:
    void operator()(std::function<void(void)> function)const;
    template<typename R,typename... Args>
    void operator()(std::function<R(Args...)>,Args&&... args)const;
};

template<size_t COUNT>
void BenchMark<COUNT>::operator()(std::function<void(void)> function)const{
	auto start = std::chrono::system_clock::now();

	for(size_t i=0;i<COUNT; ++i)function();

	auto end = std::chrono::system_clock::now();
	
	auto diff = end - start;
	std::cout << "===========BENCHMARK RESULT===========\n";
	std::cout << "    [COUNT] ";
	std::cout.width(12);
	std::cout << COUNT << " times\n";
	std::cout << "[FULL TIME] ";
	std::cout.width(12);
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " msec\n";
	std::cout << " [PER TIME] ";
	std::cout.width(12);
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() / static_cast<double>(COUNT) << " msec/function \n";
	std::cout << "======================================\n";
}

template<size_t COUNT>
template<typename R,typename... Args>
void BenchMark<COUNT>::operator()(std::function<R(Args...)> function,Args&&... args)const{
    auto start = std::chrono::system_clock::now();
    
    for(size_t i=0;i<COUNT; ++i)function(std::forward<Args>(args)...);
    
    auto end = std::chrono::system_clock::now();
    
    auto diff = end - start;
    std::cout << "===========BENCHMARK RESULT===========\n";
    std::cout << "    [COUNT] ";
    std::cout.width(12);
    std::cout << COUNT << " times\n";
    std::cout << "[FULL TIME] ";
    std::cout.width(12);
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " msec\n";
    std::cout << " [PER TIME] ";
    std::cout.width(12);
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() / static_cast<double>(COUNT) << " msec/function \n";
    std::cout << "======================================\n";
}





#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

void Timewait(std::size_t msec){
	std::chrono::time_point<std::chrono::system_clock> begin  = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> future(begin + std::chrono::milliseconds(msec));
	std::stringstream ss;
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-begin).count();

	do{
		ss.str("");
		begin  = std::chrono::system_clock::now();
		int percent = static_cast<int>(((msec - std::chrono::duration_cast<std::chrono::milliseconds>(future - begin).count()) / static_cast<double>(msec))*100);
		ss << "[TimeWating...  " << percent << "% : " << msec - std::chrono::duration_cast<std::chrono::milliseconds>(future - begin).count() << " / " << msec << "]";
		std::cout << ss.str() << std::flush;
		for(int i=0;i<std::string(ss.str()).size();i++)std::cout << "\b";
	}while(begin < future);

	for(int i=0;i<std::string(ss.str()).size();i++)std::cout << " ";
	for(int i=0;i<std::string(ss.str()).size();i++)std::cout << "\b";
	std::cout << std::flush;

	
}

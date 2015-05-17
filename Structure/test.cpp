
#include "Block.h"
#include "Field.h"
#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Heuristic.h"

class Addition:public Heuristics<int,int,int>{
public:
	int Execution(const int& a,const int& b){
		return a+b;
	}
	
};


int main(){

	Problem prob("../Problem/Problem.txt");
	Answer ans;
	Solver* solv;
	Heuristics<int,int,int>* hs;
	hs = new Addition();

	ans.SetField(prob.GetField());
	ans.AddBlocks(prob.GetBlock(0),Point(1,1),false,Constants::ANGLE90);

	std::cout << prob.GetField() << std::endl;
	std::cout << ans.GetField()  << std::endl;
	std::cout << ans             << std::endl;
	std::cout << hs->Execution(10,10) << std::endl;

	return 0;
}


#include <iostream>
#include <fstream>
#include "Problem.h"
#include "Answer.h"
#include "Solver.h"
#include "Matrix.hpp"
#include "../Utility/BenchMark.hpp"

int main(){

	Problem prob("../Problem/Problem.txt");
	std::ofstream ofs("Answer.txt");
	Field field = prob.GetField();
	Answer ans(prob);
	Solver* solv;
	Block multi;

	std::cout << std::boolalpha;
	//ofs << std::boolalpha;

	std::cout << field << std::endl;
	for(int i=0;i<prob.Count();i++){
		//std::cout << prob.GetBlock(i) << std::endl;
	}
	
	//((2,1),180,true)
	//((-3,3),180,true
	//((5,4),270,false))

	field.Projection(prob.GetBlock(0),Transform(Point(-3,-1),Constants::ANGLE0 ,false));
	
	Transform first_trans(Point(3,2),Constants::ANGLE0,false);
	
	Field f;
	f.Projection(prob.GetBlock(0)).Move(Point(3,2));
	
	BenchMark<10>()([&]{
		Field _f = f;
		f.GetListLayPossible(prob.GetBlock(1));
	});
	
	/*
	===========BENCHMARK RESULT===========
	    [COUNT]           10 times
	[FULL TIME]         4965 msec
	 [PER TIME]        496.5 msec/function 
	======================================
	*/

	/*
	std::ofstream ost("out.txt");
	std::cout << list.size() << std::endl;
	for(Transform& t:list){
		Field f = prob.GetBlock(1).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(t);
		if(!field.Cross(f)){
			auto field = prob.GetField();
			field.Projection(prob.GetBlock(0),first_trans);
			ost << t << std::endl;
			ost << (field | f) << std::endl;
		}
	}
	*/
	//field.Projection(prob.GetBlock(1),Transform(Point(-2,-3),Constants::ANGLE90,true));

	//field.Projection(prob.GetBlock(3),Transform(Point(6,0),Constants::ANGLE270,false));
	
	//std::cout << field << std::endl;

/*
	//std::cout << prob.GetBlock(1).count() << std::endl;
	//std::cout << (Block() != Block()) << std::endl;
	
	for(int i=0;i<255;i++){
		//std::vector<Transform> hands = field.GetListLayPossible(prob.GetBlock(i));
		//for(const Transform& hand:hands){
			Field _field = field;
			
			//_field.Projection(hand);
			//ofs << hand << std::endl;
			std::cout << _field.Move(Point(2,3)) << std::endl;
			break;
		//}
	}
	
	//std::cout << ans;
*/
	return 0;
}

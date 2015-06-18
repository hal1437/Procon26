
#include <iostream>
#include <fstream>
#include <random>
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
	std::random_device rd;


	Block sample[2][4];
	while(1){
		Transform rand_trans(Point((rd() % 15) - 8,
								   (rd() % 15) - 8),
							 static_cast<Constants::ANGLE>((rd()%4)*90),
							 rd()%2);
		auto b = prob.GetBlock(2).GetTransform(rand_trans);
		
		bool f =false;
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){
				Point pos(rand_trans.pos + Point(j,i));
				if(prob.GetBlock(2).GetTransform(Transform(Point(0,0),rand_trans.angle,rand_trans.reverse))[i][j]){
					if(pos.x<0 || pos.x>=BLOCK_WIDTH ||
					   pos.y<0 || pos.y>=BLOCK_HEIGHT )f =true;
				}
			}
		}
		if(f)continue;

		std::cout << rand_trans << std::endl;
		std::cout << b << std::endl;
		std::cout << b.Normalize() << std::endl;
		for(int i=0;i<1000000;i++);
	}

	/*
	std::cout << field << std::endl;
	field.Projection(prob.GetBlock(0),Transform(Point(-1,0),Constants::ANGLE0,false));
	std::cout << field << std::endl;
	field.ReverseProjection(prob.GetBlock(0),Transform(Point(-1,0),Constants::ANGLE0,false));
	std::cout << field << std::endl;
	*/

	/*
	field.Projection(prob.GetBlock(0),Transform(Point(1,0),Constants::ANGLE0 ,false));
	std::cout << field << std::endl;
	
	for(Transform f: Field(field ^ prob.GetField()).GetListLayPossible(prob.GetBlock(1))){
		std::cout << f << std::endl;
		std::cout << (field | prob.GetBlock(1).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(f)) << std::endl;
	}
	*/

	Transform first_trans(Point(3,2),Constants::ANGLE0,false);
	
	return 0;
}

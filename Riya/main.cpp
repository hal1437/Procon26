//
//  main.cpp
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/12.
//  Copyright © 2015年 Riya.Liel. All rights reserved.
//

#include "SA/SASolver.hpp"
#include "block_sa_solver.h"
#include "../Structure/Problem.h"

int main(){
    
    const std::string quest = "quest1";
    Answer_history<Transform, Block> ans;
    
    Problem problem(quest);
    Block_SA solver(ans);

    SA_Solver<Block_SA,1,99> sa_solver(solver);
    
    solver = sa_solver.solveAnswer();
    
    solver.getState().TranslateAnswer(problem).Export("Answer.txt");
    
    return 0;
}
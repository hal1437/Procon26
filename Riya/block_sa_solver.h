//
//  block_sa_solver.h
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26__block_sa_solver__
#define __Procon26__block_sa_solver__

#include"SA/SASolver.hpp"
#include"SA/SABase.hpp"
#include "AnswerHistory.h"
#include"../Structure/Problem.h"
#include"../Structure/Matrix.hpp"
#include"../Structure/Transform.h"
#include "../Structure/Heuristic.h"
#include "../Hal/Heuristics/WeightComposit.h"
#include "../Hal/Heuristics/Cavity.h"
#include "../hal/Heuristics/DensityAround.h"

class Block_SA: SA_Base<Block_SA,Problem,Field>{
public:
    Block_SA& turnState(auxType& problem);
    int calcEvalution(auxType& problem);
    
    Block_SA(stateType state);
    
private:
    Answer_history<Transform, Block> _history;
    Heuristics<unsigned,Field>* _heuristics;
    Heuristics<unsigned,Field>* _Cavity;
};



#endif /* defined(__Procon26__block_sa_solver__) */

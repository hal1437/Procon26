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
#include "../Structure/Answer.h"
#include "../Hal/Heuristics/WeightComposit.h"
#include "../Hal/Heuristics/Cavity.h"
#include "../hal/Heuristics/DensityAround.h"

class Block_SA: public SA_Base<Block_SA,Problem,Answer_history<Transform, Block>>{
public:
    Block_SA& turnState(auxType& problem);
    Block_SA& initState(auxType& problem);
    int calcEvalution(auxType& problem);
    
    explicit Block_SA(stateType state);
    Block_SA(Block_SA& rhs)=default;
    //Block_SA()=default;
    
    Field getField(){return _field;}
    
private:
    Field _field;
    Heuristics<double,Field>* _heuristics;
    Heuristics<unsigned, Field>* _Cavity;
};




#endif /* defined(__Procon26__block_sa_solver__) */

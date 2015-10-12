//
//  block_sa_solver.h
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26__block_sa_solver__
#define __Procon26__block_sa_solver__

#include "SA/SASolver.hpp"
#include "SA/SABase.hpp"
#include "AnswerHistory.h"
#include "../Structure/Problem.h"
#include "../Structure/Matrix.hpp"
#include "../Structure/Transform.h"
#include "../Structure/Heuristic.h"
#include "../Structure/Answer.h"
#include "../Hal/Heuristics/WeightComposit.h"
#include "../Hal/Heuristics/Cavity.h"
#include "../Hal/Heuristics/DensityAround.h"
#include "../Hal/Heuristics/MinArea.h"
#include "../Hal/Heuristics/Frame.h"
//#include "petternMatching.h"

class Block_SA: public SA_Base<Block_SA,Problem,Answer_history<Transform, Block>>{
public:
    //SA Interfaces
    Block_SA&   turnState       (auxType& problem);
    Block_SA&   initState       (auxType& problem);
    int         calcEvalution   (auxType& problem);
    
    double      Board_Eval   (auxType& problem);
    
    explicit Block_SA(stateType state);
    Block_SA(Block_SA& rhs)=default;
    //Block_SA()=default;
    
    Field getField(){return _field;}
    
private:
    typedef std::pair<double,Transform> HAND_PAIR;
    
    std::vector< HAND_PAIR > next_evals;
    std::vector< int >       hand_iterator;
    std::vector< Transform > reserveTrans;
    std::vector< int >       remain_number_of_blocks;
    
    Field _field;
    Heuristics<double,Field,Problem>* _heuristics;  
};




#endif /* defined(__Procon26__block_sa_solver__) */

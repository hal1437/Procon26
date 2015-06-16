//
//  block_sa_solver.cpp
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "block_sa_solver.h"
#include<random>

Block_SA& Block_SA::turnState(auxType& aux){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distribution(0,_history.size()-1);
    std::vector<Transform> hands;
    
    auto field = aux.GetField();
    
    _history.returnTheHand(distribution(mt), _state);
    
    for(int i=_history.size(); i < aux.Count(); i++){
        
        hands = aux.GetBlock(i).GetListLayPossible(field);
        
        std::max_element(hands.begin(),hands.end(),[&](const Transform& lhs,const Transform& rhs){
            Field l_field,r_field;
            l_field = r_field = field;
            l_field.Projection(aux.GetBlock(i),lhs);
            r_field.Projection(aux.GetBlock(i),rhs);
            return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
        });

    }
    
    return *this;
}

int Block_SA::calcEvalution(auxType& aux){
    int eval;
    
    return eval;
}

Block_SA::Block_SA(stateType state) : SA_Base<Block_SA, Problem, Field>(state){
    auto density = new WeightComposit();
    density->AddHeuristic(new Cavity(),-1.0f);
    
    _heuristics = density;
}
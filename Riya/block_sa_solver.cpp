//
//  block_sa_solver.cpp
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "block_sa_solver.h"
#include<random>

Block_SA& Block_SA::turnState(auxType& problem){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distribution(0,_history.size()-1);
    std::vector<Transform> hands;
    
    auto field = _state;
    
    _history.returnTheHand(distribution(mt), _state);
    
    for(int i=_history.size(); i < problem.Count(); i++){
        
        hands = problem.GetBlock(i).GetListLayPossible(field);
        
        hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& n){
                        auto tmp = field;
                        tmp.Projection(problem.GetBlock(i),n);
                        return _Cavity->Execution(tmp) != 0;
                    }),
                    hands.end()
                    );
        
        if(hands.empty()){
            _history.push_back( std::make_pair(Transform(), problem.GetBlock(i)) );
        }
        else{
            auto hand = hands[mt() % hands.size()];
            
            _state.Projection(problem.GetBlock(i) , hand);
            _history.push_back( std::make_pair(hand, problem.GetBlock(i)) );
        }
        
        std::cout << _state << "¥n" << std::endl;

    }
    
    std::cout << _state << std::endl;
    
    return *this;
}

int Block_SA::calcEvalution(auxType& problem){
    return _heuristics->Execution(_state);

}

Block_SA::Block_SA(stateType state) : SA_Base<Block_SA, Problem, Field>(state){
    auto density = new WeightComposit();
    
    density->AddHeuristic(new DensityAround(),1.0f);
    density->AddHeuristic(new Cavity(),-12.0f);
    
    _heuristics = density;
    _Cavity = new Cavity;
}
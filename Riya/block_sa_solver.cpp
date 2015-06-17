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
        
        hands = _state.GetListLayPossible(problem.GetBlock(i));
        
        if(hands.empty()){
            _history.push_back( std::make_pair(Transform(), problem.GetBlock(i)) );
            continue;
        }
        
        auto best = *std::max_element(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
            Field l_field,r_field;
            l_field = r_field = _state;
            l_field.Projection(problem.GetBlock(i),lhs);
            r_field.Projection(problem.GetBlock(i),rhs);
            return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
        });

        hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& n){
                        auto tmp = _state;
                        tmp.Projection(problem.GetBlock(i),n);
                        return _Cavity->Execution(tmp) > _Cavity->Execution(_state);
                    }),
                    hands.end()
                    );
        
        if(hands.empty()){
            _history.push_back( std::make_pair(best, problem.GetBlock(i)) );
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

Block_SA& Block_SA::initState(auxType& problem){
    std::vector<Transform> hands;
    const int count = problem.Count();
    
    _state =problem.GetField();
    
    std::cout << _state << std::endl;
    
    for(int i=0; i<count; i++){
        
        std::cout << problem.GetBlock(i) << std::endl;
        hands = _state.GetListLayPossible(problem.GetBlock(i));
        
        if(hands.empty())_history.push_back( std::make_pair(Transform(), problem.GetBlock(i)) );
        else{
        
            auto best = *std::max_element(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
                Field l_field,r_field;
                l_field = r_field = _state;
                l_field.Projection(problem.GetBlock(i),lhs);
                r_field.Projection(problem.GetBlock(i),rhs);
                return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
            });
            
            _state.Projection(problem.GetBlock(i), best);
            _history.push_back(std::make_pair(best, problem.GetBlock(i)));
            
        }
        
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
    density->AddHeuristic(new Cavity(),-5.0f);
    
    _heuristics = density;
    
    _Cavity = new Cavity();
    
}
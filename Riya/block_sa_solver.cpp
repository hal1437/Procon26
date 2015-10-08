//
//  block_sa_solver.cpp
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "block_sa_solver.h"
#include "Contain.hpp"
#include<random>

Block_SA& Block_SA::turnState(auxType& problem){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distribution;
    std::vector<Transform> hands;
    
    std::size_t r = _state.search_bad_index();
    
    Contain contain(problem);
    
    _state.returnTheHand(r ,_field);
    std::cout << "restart hand number = " << r << " evalution = " << _state.get_eval(r) <<std::endl;
    std::cout << (_field | problem.GetField()) << std::endl;
    
    for(long i=r; i < problem.Count(); i++){
        hands = _field.GetListLayPossible(problem.GetBlock(i),problem.GetField(),i==0);
        
        if(hands.empty()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1), i);
            continue;
        }
        
        next_evals.resize(hands.size());
        for(int j=0; j<hands.size(); j++){
            const Field new_field = static_cast<Field>(_field | problem.GetField()).GetProjection(problem.GetBlock(i),hands[j]);
            next_evals[j] = std::make_pair(_heuristics->Execution(static_cast<Field>(new_field |
                                    problem.GetField()),problem),hands[j]);
           next_evals[j].first -= (contain.Execution(static_cast<Field>(_field | problem.GetField()), i)) ? -10000 : 0;
        }
        
        std::sort(next_evals.begin(), next_evals.end(), [&](const HAND_PAIR& lhs,const HAND_PAIR& rhs){
            return lhs.first < rhs.first;
        });
        
        auto& selected_haad = next_evals[std::uniform_int_distribution<long>(0,next_evals.size())(mt)];
        _field.Projection(problem.GetBlock(i), selected_haad.second);
        _state.set_ans(std::make_pair(selected_haad.second, problem.GetBlock(i)),i);
        _state.set_eval( Board_Eval(problem),i );

    }
    
    std::cout << (_field | problem.GetField()) << std::endl;
    return *this;
}

Block_SA& Block_SA::initState(auxType& problem){
    std::vector<Transform> hands;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    
    const int count = problem.Count();
    
    std::cout << _field << std::endl;
    
    for(int i=0; i<count; i++){
        hands = _field.GetListLayPossible(problem.GetBlock(i),problem.GetField(),i==0);
        
        if(hands.empty()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1), i);
            continue;
        }
        
        next_evals.resize(hands.size());
        for(int j=0; j<hands.size(); j++){
            next_evals[j] = std::make_pair( _heuristics->Execution(static_cast<Field>(_field | problem.GetField()).GetProjection(problem.GetBlock(i),hands[j]),problem)
                                           , hands[j] );
        }
        auto best = std::max_element(next_evals.begin(), next_evals.end(), [&](const HAND_PAIR& lhs,const HAND_PAIR& rhs){
            return lhs.first < rhs.first;
        });
        
        _field.Projection(problem.GetBlock(i), best->second);
        _state.set_ans(std::make_pair(best->second, problem.GetBlock(i)),i);
        _state.set_eval( Board_Eval(problem),i );
        std::cout << i << "th" << std::endl;
        std::cout << (_field | problem.GetField()) << std::endl;
    }
    std::cout << (_field | problem.GetField()) << std::endl;
    return *this;
}

double Block_SA::Board_Eval(auxType& problem){
    return _heuristics->Execution(_field | problem.GetField(), problem);
}

int Block_SA::calcEvalution(auxType& problem){
    return (_field | problem.GetField()).count(); //TODO valid stone count
}

Block_SA::Block_SA(stateType state) : SA_Base<Block_SA, Problem, Answer_history<Transform, Block>>(state){
    auto density = new WeightComposit();
    
    density->AddHeuristic(new DensityAround(),1.0f);
    density->AddHeuristic(new MinArea(), -1.0f);
    //density->AddHeuristic(new AntiDensityAround(), 0.9f);
    //density->AddHeuristic(new Frame(), 0.1f);
    
    _heuristics = density;
    
    _Cavity = new Cavity();
    
}
//
//  block_sa_solver.cpp
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "block_sa_solver.h"
#include "PerfectContain.hpp"
#include<random>

Block_SA& Block_SA::turnState(auxType& problem){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distribution;
    std::vector<Transform> hands;
    
    PerfectContain *p_contain = new PerfectContain(problem);
    
    std::size_t r = _state.search_bad_index();
    
    _state.returnTheHand(r ,_field);
    std::fill(hand_iterator.begin()+r,hand_iterator.end(),0);
    
    for(int i=r; i<reserveTrans.size(); i++){
        reserveTrans[i] = Transform();
    }
    
    std::cout << (_field | problem.GetField()) << std::endl;
    std::cout << "restart hand number = " << r << " evalution = " << _state.get_eval(r) <<std::endl;
    
    for(long i=r; i < problem.Count(); i++){
        if(p_contain->isEnableReserve){
            if(reserveTrans[i].isEnable()){
                _state.set_ans(std::make_pair(reserveTrans[i], problem.GetBlock(i)),i);
                _state.set_eval( Board_Eval(problem), i);
                continue;
            }
        }
        
        std::cout << (_field | problem.GetField()) << std::endl;
        if(i!=0 && !p_contain->Execution(_field, _state.get_ans(i-1).first, i-1, reserveTrans))return *this;
        std::cout << (_field | problem.GetField()) << std::endl;
        
        hands = _field.GetListLayPossible(problem.GetBlock(i),problem.GetField(),i==0);

        next_evals.clear();
        for(int j=0; j<hands.size(); j++){
            Field new_field = static_cast<Field>(_field | problem.GetField()).GetProjection(problem.GetBlock(i),hands[j]);
            next_evals.push_back(std::make_pair(_heuristics->Execution(static_cast<Field>(new_field |
                                    problem.GetField()),problem),hands[j]));
            if(i!=0 && static_cast<Field>(~(new_field | problem.GetField())).count() <= remain_number_of_blocks[j] && !p_contain->Execution(new_field,hands[j],i)){
                hands.erase(hands.begin() + j); j--;
                continue;
            }

        }
        
        if(hands.empty() || hand_iterator[i] > next_evals.size()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1), i);
            continue;
        }
        
        std::sort(next_evals.begin(), next_evals.end(), [&](const HAND_PAIR& lhs,const HAND_PAIR& rhs){
            return lhs.first > rhs.first;
        });
        
        std::cout << (_field | problem.GetField()) << std::endl;
        auto& selected_hand = next_evals[hand_iterator[i]];
        hand_iterator[i]++;
        _field.Projection(problem.GetBlock(i), selected_hand.second);
        _state.set_ans(std::make_pair(selected_hand.second, problem.GetBlock(i)),i);
        _state.set_eval( Board_Eval(problem),i );
    }
    
    std::cout << (_field | problem.GetField()) << std::endl;
    std::cout << "turned state. evalution = " << calcEvalution(problem) <<std::endl;
    return *this;
}

Block_SA& Block_SA::initState(auxType& problem){
    std::vector<Transform> hands;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    
    PerfectContain *p_contain = new PerfectContain(problem);
    
    reserveTrans.resize(problem.Count());
    hand_iterator.resize(problem.Count());
    remain_number_of_blocks.resize(problem.Count());
    
    std::fill(hand_iterator.begin(),hand_iterator.end(),0);
    for(auto& trans: reserveTrans){
        trans = Transform();
    }
    
    int sum=0;
    
    for(std::size_t i=0; i<problem.Count(); i++)sum += problem.GetBlock(i).count();
    
    for(long i=0; i < problem.Count(); i++){
        
        sum -= problem.GetBlock(i).count();
        remain_number_of_blocks[i] = sum;
        
        if(p_contain->isEnableReserve){
            if(reserveTrans[i].isEnable()){
                _state.set_ans(std::make_pair(reserveTrans[i], problem.GetBlock(i)),i);
                _state.set_eval( Board_Eval(problem), i);
                continue;
            }
        }
        
        std::cout << (_field | problem.GetField()) << std::endl;
        if(i!=0 && !p_contain->Execution(_field, _state.get_ans(i-1).first, i-1, reserveTrans))return *this;
        std::cout << (_field | problem.GetField()) << std::endl;
        
        hands = _field.GetListLayPossible(problem.GetBlock(i),problem.GetField(),i==0);
        
        next_evals.clear();
        for(int j=0; j<hands.size(); j++){
            Field new_field = static_cast<Field>(_field | problem.GetField()).GetProjection(problem.GetBlock(i),hands[j]);
            if(i!=0 && static_cast<Field>(~(new_field | problem.GetField())).count() <= remain_number_of_blocks[j] && !p_contain->Execution(new_field,hands[j],i)){hands.erase(hands.begin() + j); j--; continue;}
            next_evals.push_back(std::make_pair(_heuristics->Execution(static_cast<Field>(new_field |
                                                                                          problem.GetField()),problem),hands[j]));
        }
        
        if(hands.empty() || hand_iterator[i] > next_evals.size()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1), i);
            continue;
        }
        
        std::sort(next_evals.begin(), next_evals.end(), [&](const HAND_PAIR& lhs,const HAND_PAIR& rhs){
            return lhs.first > rhs.first;
        });
        
        auto& selected_hand = next_evals.front();
        _field.Projection(problem.GetBlock(i), selected_hand.second);
        std::cout << (_field | problem.GetField()) << std::endl;
        _state.set_ans(std::make_pair(selected_hand.second, problem.GetBlock(i)),i);
        _state.set_eval( Board_Eval(problem),i );
    }
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
    density->AddHeuristic(new AntiDensityAround(), 0.9f);
    //density->AddHeuristic(new Frame(), 0.1f);
    
    _heuristics = density;
}

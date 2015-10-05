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
    std::uniform_int_distribution<int> distribution(0,_state.size()-1);
    std::vector<Transform> hands;
    
    int r = distribution(mt);
    
    _state.returnTheHand(r ,_field);
    std::cout << "restart hand number = " << r << std::endl;
    std::cout << (_field | problem.GetField()) << std::endl;
    
    for(long i=r; i < problem.Count(); i++){
        
        if(i==0){
            while(1){
                Transform rand_trans(Point((mt() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
                                           (mt() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
                                     static_cast<Constants::ANGLE>((mt()%4)*90),
                                     mt()%2);
                auto b = problem.GetBlock(0).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(rand_trans);
                bool f =false;
                for(int i=0;i<FIELD_HEIGHT;i++){
                    for(int j=0;j<FIELD_WIDTH;j++){
                        Point pos(rand_trans.pos + Point(i,j));
                        if(b[i][j]){
                            if(pos.x<0 || pos.x>FIELD_WIDTH ||
                               pos.y<0 || pos.y>FIELD_HEIGHT )f =true;
                        }
                    }
                }
                if(f)continue;
                std::cout << b << std::endl;
                if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
                    _state.set_ans(std::make_pair(rand_trans, problem.GetBlock(0)),i);
                    _field.Projection(problem.GetBlock(0),rand_trans);
                    break;
                }
            }
            continue;
        }
        
        hands = _field.GetListLayPossible(problem.GetBlock(i));
        hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& trans){
            return ((problem.GetField() & problem.GetBlock(i).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(trans)).count()>0);
        }),hands.end());
        
        if(hands.empty()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1),i );
        }
        else{
            std::sort(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
                Field l_field,r_field;
                l_field = r_field = _field | problem.GetField();
                l_field.Projection(problem.GetBlock(i),lhs);
                r_field.Projection(problem.GetBlock(i),rhs);
                return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
            });
            
            auto hand = hands[mt() % hands.size()];
            
            _field.Projection(problem.GetBlock(i) , hand);
            _state.set_ans( std::make_pair(hand, problem.GetBlock(i)),i );
            _state.set_eval( calcEvalution(problem),i );
        }
    }
    
    std::cout << (_field | problem.GetField()) << std::endl;
    std::cout << 'a'<< std::endl;
    return *this;
}

Block_SA& Block_SA::initState(auxType& problem){
    std::vector<Transform> hands;
    std::random_device rnd;
    std::mt19937 mt(rnd());
    
    const int count = problem.Count();
    
    std::cout << _field << std::endl;
    
    for(int i=0; i<count; i++){
        if(i==0){
            while(1){
                Transform rand_trans(Point((mt() % (FIELD_WIDTH +BLOCK_WIDTH )) - BLOCK_WIDTH ,
                                           (mt() % (FIELD_HEIGHT+BLOCK_HEIGHT)) - BLOCK_HEIGHT),
                                     static_cast<Constants::ANGLE>((mt()%4)*90),
                                     mt()%2);
                auto b = problem.GetBlock(0).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(rand_trans);
                bool f = false;
                for(int i=0;i<FIELD_HEIGHT;i++){
                    for(int j=0;j<FIELD_WIDTH;j++){
                        Point pos(rand_trans.pos + Point(i,j));
                        if(b[i][j]){
                            if(pos.x<0 || pos.x>FIELD_WIDTH ||
                               pos.y<0 || pos.y>FIELD_HEIGHT )f =true;
                        }
                    }
                }
                if(f)continue;
                if((problem.GetField() & b).count() == 0 && b.count() == problem.GetBlock(0).count()){
                    _state.set_ans(std::make_pair(rand_trans, problem.GetBlock(0)),i);
                    _state.set_eval(calcEvalution(problem),i );
                    _field.Projection(problem.GetBlock(0),rand_trans);
                    break;
                }
            }
            continue;
        }
        
        hands = _field.GetListLayPossible(problem.GetBlock(i));
        hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& trans){
            return ((problem.GetField() & problem.GetBlock(i).GetTransform<FIELD_WIDTH,FIELD_HEIGHT>(trans)).count()>0);
        }),hands.end());
        
        if(hands.empty()){
            _state.set_ans( std::make_pair(Transform(), problem.GetBlock(i)),i );
            _state.set_eval( _state.get_eval(i-1), i);
        }
        else{
            
            auto best = *std::max_element(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
                Field l_field,r_field;
                l_field = r_field = _field | problem.GetField();
                l_field.Projection(problem.GetBlock(i),lhs);
                r_field.Projection(problem.GetBlock(i),rhs);
                return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
            });
            
            _field.Projection(problem.GetBlock(i), best);
            _state.set_ans(std::make_pair(best, problem.GetBlock(i)),i);
            _state.set_eval( calcEvalution(problem),i );
        }
        
    }
    
    std::cout << (_field | problem.GetField()) << std::endl;
    return *this;
}

int Block_SA::calcEvalution(auxType& problem){
    return _heuristics->Execution(_field | problem.GetField());
}

Block_SA::Block_SA(stateType state) : SA_Base<Block_SA, Problem, Answer_history<Transform, Block>>(state){
    auto density = new WeightComposit();
    
    density->AddHeuristic(new DensityAround(),1.0f);
    density->AddHeuristic(new Cavity(),-5.0f);
    
    _heuristics = density;
    
    _Cavity = new Cavity();
    
}
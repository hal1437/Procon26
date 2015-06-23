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
    
    _state.returnTheHand(distribution(mt), _field);
    
    std::cout << (_field | problem.GetField()) << std::endl;
    
    for(int i=_state.size(); i < problem.Count(); i++){
        
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
                    _state.push_back(std::make_pair(rand_trans, problem.GetBlock(0)));
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
            _state.push_back( std::make_pair(Transform(), problem.GetBlock(i)) );
            continue;
        }
        
        auto best = *std::max_element(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
            Field l_field,r_field;
            l_field = r_field = _field | problem.GetField();
            l_field.Projection(problem.GetBlock(i),lhs);
            r_field.Projection(problem.GetBlock(i),rhs);
            return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
        });
        
        hands.erase(std::remove_if(hands.begin(),hands.end(),[&](const Transform& n){
                        Field tmp = _field | problem.GetField();
                        tmp.Projection(problem.GetBlock(i),n);
                        return _Cavity->Execution(tmp) > _Cavity->Execution(_field);
                    }),
                    hands.end()
                    );
        
        if(hands.empty()){
            _state.push_back( std::make_pair(best /*Transform()*/, problem.GetBlock(i)) );
        }
        else{
            auto hand = hands[mt() % hands.size()];
            
            _field.Projection(problem.GetBlock(i) , hand);
            _state.push_back( std::make_pair(hand, problem.GetBlock(i)) );
        }
        
        std::cout << _field << "¥n" << std::endl;

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
        std::cout << _field << std::endl;
        std::cout << problem.GetBlock(i) << std::endl;
        
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
                    _state.push_back(std::make_pair(rand_trans, problem.GetBlock(0)));
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
        
        if(hands.empty())_state.push_back( std::make_pair(Transform(), problem.GetBlock(i)) );
        else{
            
            auto best = *std::max_element(hands.begin(), hands.end(), [&](const Transform& lhs,const Transform& rhs){
                Field l_field,r_field;
                l_field = r_field = _field | problem.GetField();
                l_field.Projection(problem.GetBlock(i),lhs);
                r_field.Projection(problem.GetBlock(i),rhs);
                return _heuristics->Execution(l_field) < _heuristics->Execution(r_field);
            });
            
            _field.Projection(problem.GetBlock(i), best);
            _state.push_back(std::make_pair(best, problem.GetBlock(i)));
            
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
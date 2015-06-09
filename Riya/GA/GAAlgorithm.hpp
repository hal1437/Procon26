//
//  GAAlgorithm.hpp
//  Procon26
//
//  Created by Riya.Liel on 2015/06/01.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __GA_GAAlgorithm_hpp__
#define __GA_GAAlgorithm_hpp__

#include <vector>
#include <cmath>
#include <random>
#include <utility>

template<class _Individual>
struct RouletteSelect{
    /*_Individual* operator()(std::vector<_Individual*>& _population){
        _Individual* individual=nullptr;
        
        double rnd = normalized_number;
        
        for(int i=0;i<_population.size();i++){
            if(_population[i]->getProbability() > rnd){
                individual = _population[i];
                break;
            }
        }
        return individual;
    }*/
    _Individual* operator()(std::vector<_Individual*>&& _population){
        _Individual* individual=nullptr;
        
        double rnd = _distribution(_mt);
        
        for(int i=0;i<_population.size();i++){
            if(_population[i]->getProbability() > rnd){
                individual = _population[i];
                break;
            }
        }
        return individual;
    }
    static std::uniform_real_distribution<double> _distribution;
    static std::mt19937 _mt;
};

template<class _Individual>
std::uniform_real_distribution<double> RouletteSelect<_Individual>::_distribution = std::uniform_real_distribution<double>(0,1);

template<class _Individual>
std::mt19937 RouletteSelect<_Individual>::_mt = std::mt19937(std::random_device()());

template<class _Individual>
struct PowerScaling{
    void operator()(std::vector<_Individual*>& _population,int exponent){
        for(auto I: _population){
            I->setEvalution( pow(I->getEvalution(),exponent) );
        }
    }
    void operator()(std::vector<_Individual*>&& _population,int exponent){
        for(auto I: _population){
            I->setEvalution( pow(I->getEvalution(),exponent) );
        }
    }
};

#endif

//
//  modules.h
//  opencv_test
//
//  Created by Riya.Liel on 2015/05/19.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef opencv_test_modules_hpp
#define opencv_test_modules_hpp

#include "GABase.h"
#include <vector>
#include <algorithm>

void populationSettings(std::vector<GABase*> population){
    
    std::sort(population.begin(), population.end(), [](GA_Base* lhs,GA_Base* rhs){return lhs->getEvalution() < rhs->getEvalution();});
    
    int under = population.back()->getEvalution();
    long sum=0;
    double sumProbablity;
    
    for(int individual : population){
        individual->setEvalution(individual->getEvalution() - under);
        sum+=individual;
    }
    
    for(int individual : population){
        individual->setProbability(individual->getEvalution()/sum + sumProbablity);
        sumProbablity += individual->getProbability();
    }
}

#endif

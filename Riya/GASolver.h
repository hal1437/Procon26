//
//  GASolver.h
//  Procon26
//
//  Created by Riya.Liel on 2015/05/20.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26__GASolver__
#define __Procon26__GASolver__

#include "GABase.h"
#include <random>

class GA_Solver{
public:
    GA_Solver(std::vector<GA_Base*> solve_targets);
    GA_Base* select();
    GA_Base* solveAnswer(int);
    
    void populationSettings();
    
    const int NUMBER_OF_INDIVIDUAL=100;
    const double PROBABILITY_OF_MUTATION=0.05;
    const double PROBABILITY_OF_INVERSION=0.03;
private:
    void *aux;
    std::random_device rnd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> distribution;
    std::vector<GA_Base*> population;
};

#endif /* defined(__Procon26__GASolver__) */

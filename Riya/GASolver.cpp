//
//  GASolver.cpp
//  Procon26
//
//  Created by Riya.Liel on 2015/05/20.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#include "GASolver.h"
#include <algorithm>

GA_Solver::GA_Solver(std::vector<GA_Base*> solve_targets){
    population = solve_targets;
    mt = std::mt19937(rnd());
    distribution = std::uniform_real_distribution<double>(0,1);
}

GA_Base* GA_Solver::select(){
    GA_Base* individual=nullptr;
    
    double rnd = distribution(mt);
    
    for(int i=0;i<population.size();i++){
        if(population[i]->getProbability() < rnd){
            individual = population[i];
        }
    }
    
    return individual;
}
GA_Base* GA_Solver::solveAnswer(int max_age){
    GA_Base* answer;
    std::vector<GA_Base*> new_poplation;
    
    for(int i=0;i<max_age;i++){
        new_poplation.clear();
        populationSettings();
        new_poplation.push_back(population.front());
        
        while(new_poplation.size() >= NUMBER_OF_INDIVIDUAL){
            GA_Base* father = select(),*mother = select(),*target = select();
            new_poplation.push_back(father->cross_over(mother));
            if(distribution(mt) < PROBABILITY_OF_MUTATION){
                target->mutation();
                new_poplation.push_back(target);
            }
            else if(distribution(mt) < PROBABILITY_OF_INVERSION){
                target->inversion();
                new_poplation.push_back(target);
            }
        }
        
        population = new_poplation;
    
    }
    
    return answer;
}

void GA_Solver::populationSettings(){
    for(GA_Base* individual : population){
        individual->calcEvalution(aux);
    }
    std::sort(population.begin(), population.end(), [](GA_Base* lhs,GA_Base* rhs){return lhs->getEvalution() < rhs->getEvalution();});
    
    int under = population.back()->getEvalution();
    long sum=0;
    double sumProbablity;
    
    for(GA_Base* individual : population){
        individual->setEvalution(individual->getEvalution() - under);
        sum+=individual;
    }
    
    for(GA_Base* individual : population){
        individual->setProbability(individual->getEvalution()/sum + sumProbablity);
        sumProbablity += individual->getProbability();
    }
}
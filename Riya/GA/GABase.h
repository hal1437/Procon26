//
//  Structure.h
//  opencv_test
//
//  Created by Riya.Liel on 2015/04/24.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __opencv_test__Structure__
#define __opencv_test__Structure__

#include <vector>
#include <array>

class GA_BASE_CLASS{ //Interface class for GA
public:
    virtual void mutation()=0;
    virtual void inversion()=0;
    virtual GA_BASE_CLASS* cross_over(GA_BASE_CLASS*)=0;
    
    bool operator<(const GA_BASE_CLASS* rhs){return this->_evalution < rhs->_evalution;}
    bool operator>(const GA_BASE_CLASS* rhs){return this->_evalution > rhs->_evalution;}

    enum class DNA_DISPLACEMENT_LISTS{
        MUTATION=0,
        INVERSION,
    };
    static const int NUMBER_OF_DISPLACEMENT=3;
    
    void setDistribution(std::array<double, NUMBER_OF_DISPLACEMENT> dist){_evolution_distribution = dist;}
    
protected:
    int _evalution;
    std::array<double, NUMBER_OF_DISPLACEMENT> _evolution_distribution;
};




#endif /* defined(__opencv_test__Structure__) */

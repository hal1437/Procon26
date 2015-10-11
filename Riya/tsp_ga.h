//
//  tsp.h
//  opencv_test
//
//  Created by Riya.Liel on 2015/04/24.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __GA_tsp__
#define __GA_tsp__

#include"GA/GABase.hpp"
#include"reference_list.h"
#include <vector>

class tsp_individual : public GA_Base<tsp_individual,std::vector<cv::Point>>{
public:
    using DNA = std::vector<int>;

    tsp_individual* mutation();
    tsp_individual* cross_over(tsp_individual*);
    tsp_individual* cse_x_cross_over(tsp_individual*);
    int calcEvalution(std::vector<cv::Point>& aux);
    
    static DNA translateToDnaPhenotypicOrdinal(const DNA);
    static DNA translateToDnaPhenotypicTrait(const DNA);
    
    void setDNA(DNA& _dna){_phenotypic_ordinal=_dna;}
    void setDNA(DNA&& _dna){_phenotypic_ordinal=_dna;}
    
    //debug
    const DNA& getPhenotypic()const{return _phenotypic_ordinal;}
    
private:
    
    DNA _phenotypic_ordinal;
};

tsp_individual* makeTspIndividual(int number_of_city);

#endif /* defined(__opencv_test__tsp__) */

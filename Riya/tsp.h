//
//  tsp.h
//  opencv_test
//
//  Created by Riya.Liel on 2015/04/24.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __opencv_test__tsp__
#define __opencv_test__tsp__

#include"GABase.h"
#include"reference_list.h"
#include <vector>

class tsp_individual : public GA_Base<std::vector<cv::Point>>{
public:
    using DNA = std::vector<int>;

    void mutation() override;
    void inversion() override;
    tsp_individual* cross_over(GA_Base*) override;
    int calcEvalution(void* aux) override;
    
    DNA translateToDnaPhenotypicOrdinal(DNA);
    DNA translateToDnaPhenotypicTrait(DNA);
    
    void setDNA(DNA& _dna){_phenotypic_trait=_dna;}
    
private:
    
    DNA _phenotypic_trait;
};

tsp_individual* makeTspIndividual(int number_of_city);

#endif /* defined(__opencv_test__tsp__) */

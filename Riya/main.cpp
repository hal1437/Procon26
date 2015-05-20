#include<iostream>
#include"tsp.h"

int main(){
    tsp_individual* indibidual = makeTspIndividual(3);
    std::vector<cv::Point> city_list={cv::Point(0,5),cv::Point(3,5),cv::Point(13,5)};
    
    std::cout << indibidual->calcEvalution(&city_list) << std::endl;
    return 0;
}
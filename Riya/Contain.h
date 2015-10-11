//
//  Contain.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/07.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#pragma once

#include"blockUtility.h"
#include"../Utility/PARALLEL.hpp"
#include"../Hal/Perfect/PerfectComposit.h"
#include"Config.h"
#include<queue>
#include<numeric>
#include<algorithm>
#include<future>

/* how to use
 
 Contain test(prob);
 Field field = prob.GetField();
 
 auto hands = field.GetListLayPossible(prob.GetBlock(0),prob.GetField(),true);
 std::vector<int> reserveBlocks(prob.Count());
 
 BenchMark<1>()([&]{
    for(auto& hand: hands){
        if(!test.Execution(field.GetProjection(prob.GetBlock(0),hand),hand,0,reserveBlocks))std::cout << "no contains" <<std::endl;
    }
});
 
 */


class Contain/* : public Perfect<Field,BlockLayer>*/{
private:
    struct square;
    typedef Perfect<Field,BlockLayer> h_type;
    typedef std::pair<square, std::vector<std::vector<int>> > closed_range;
    
    std::vector<h_type*> heuristicses;
public:
    typedef std::pair<std::vector<int>,std::vector<int>> geometry_feature;
    static constexpr bool isEnableReserve = false;
    static constexpr int  MAX_CLOSED_FIELD_SIZE = 20;
    
    bool Execution(const Field& field,const Transform& trans,std::size_t index);
    
    Contain(const Problem& prob);
    //Contain()=default;
    Contain(const Contain&)=default;
    
private:
    const Problem& _prob;
    std::vector<geometry_feature> _features;
    std::vector<std::size_t> _features_index;
    
    struct square{
        Point begin,end;
        bool operator==(const square& rhs){return this->begin == rhs.begin && this->end == rhs.end;}
    };
    
    closed_range searchClosedField(const Field& field,const Point& pos);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<std::vector<int>> trimField(const Matrix<WIDTH, HEIGHT>& field, const  square& sq);

    bool isFieldContainAllRemainBlocks(const Field& field,const std::vector<std::vector<int>>& closed,std::size_t index);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block);
    
    geometry_feature calcGeometryFeature(const std::vector< std::vector<int> >& block);
    
    geometry_feature calcGeometryFeatureField(const std::vector< std::vector<int> >& field);
    
    std::vector<Point> getReachable(const Field& field,const Block& block,const Transform& trans);
    
public:
    
    bool isContain(const std::vector< std::vector<int> >& field,geometry_feature gf);
};

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector<std::vector<int>> Contain::trimField(const Matrix<WIDTH, HEIGHT>& field, const square& sq){
    std::vector<std::vector<int>> parted_field;
    
    parted_field.resize(sq.end.y - sq.begin.y + 1);
    for(auto& cols: parted_field){
        cols.resize(sq.end.x - sq.begin.x + 1);
    }
    
    for(int i=sq.begin.y; i<=sq.end.y; i++){
        for(int j=sq.begin.x; j<=sq.end.x; j++){
            parted_field[i - sq.begin.y][j - sq.begin.x] = field[i][j];
        }
    }
    
    return parted_field;
}


template<std::size_t WIDTH,std::size_t HEIGHT>
Contain::geometry_feature Contain::calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block){
    std::vector< std::vector<int> > parted_block = particalProblem(block);
    geometry_feature gf;
    int initial_point,segment_size;
    
    for(int j=0; j<parted_block[0].size(); j++){
        initial_point=-1;
        segment_size=0;
        for(int i=0; i<parted_block.size(); i++){
            if(initial_point == -1 && parted_block[i][j] == 1)initial_point = i;
            if(initial_point != -1 && parted_block[i][j] == 1)segment_size = i - initial_point + 1;
        }
        if(segment_size > 0)gf.first.push_back(segment_size);
    }
    
    for(int i=0; i<parted_block.size(); i++){
        initial_point=-1;
        segment_size=0;
        for(int j=0; j<parted_block[i].size(); j++){
            if(initial_point == -1 && parted_block[i][j] == 1)initial_point = j;
            if(initial_point != -1 && parted_block[i][j] == 1)segment_size = j - initial_point + 1;
        }
        if(segment_size > 0)gf.second.push_back(segment_size);
    }
    
    if(gf.second.size() > gf.first.size())std::swap(gf.first,gf.second);
    
    std::sort(gf.first.begin(),gf.first.end(),std::greater<int>());
    std::sort(gf.second.begin(),gf.second.end(),std::greater<int>());
    
    return gf;
}


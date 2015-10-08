//
//  Contain.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/07.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_project_Contain_hpp
#define Procon26_project_Contain_hpp

#include"blockUtility.h"
#include"PARALLEL.hpp"
#include"../Hal/Perfect/PerfectComposit.h"
#include"Config.h"
#include<queue>
#include<algorithm>
#include<future>

class Contain/* : public Perfect<Field,BlockLayer>*/{
private:
    typedef Perfect<Field,BlockLayer> h_type;
    typedef std::pair<std::vector<int>,std::vector<int>> geometry_feature;
    
    std::vector<h_type*> heuristicses;
public:
    bool Execution(const Field& field,std::size_t index);
    template<std::size_t WIDTH,std::size_t HEIGHT>
    bool Execution(const Matrix<WIDTH, HEIGHT>&field, std::size_t index);
    
    Contain(const Problem& prob);
    //Contain()=default;
    Contain(const Contain&)=default;
    
private:
    const Problem& _prob;
    std::vector<geometry_feature> _features;
    struct square{
        Point begin,end;
    };
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector< std::vector<int> > searchClosedField(Matrix<WIDTH, HEIGHT>&field,const Point& pos);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<std::vector<int>> trimField(const Matrix<WIDTH, HEIGHT>& field, const  square& sq);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    bool isFieldContainAllRemainBlocks(const Matrix<WIDTH, HEIGHT>& field,const std::vector<std::vector<int>>& closed,std::size_t index);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block);
    
    geometry_feature calcGeometryFeature(const std::vector< std::vector<int> >& block);
    
public:
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeatureField(const Matrix<WIDTH,HEIGHT>& field);
    
    geometry_feature calcGeometryFeatureField(const std::vector< std::vector<int> >& field);
    
    bool isContain(const std::vector< std::vector<int> >& field,geometry_feature gf);
};

Contain::Contain(const Problem& prob):_prob(prob){
    _features.resize(prob.Count());;
    
    for(std::size_t i=0; i<_prob.Count(); i++){
        _features[i] = calcGeometryFeature(particalBlock(_prob.GetBlock(i)));
    }
}

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
bool Contain::isFieldContainAllRemainBlocks(const Matrix<WIDTH, HEIGHT>& field,const std::vector<std::vector<int>>& closed,std::size_t index){
    bool isFieldContain=false;
    int N = _prob.Count();
#ifdef USE_PARALLEL
    PARALLEL_FOR(index, N, isFieldContain |= isContain(closed,_features[P_IT]););
#else
    for(int i=index; i<_prob.Count(); i++){
        //std::cout << _prob.GetBlock(i) << std::endl;
        isFieldContain |= isContain(closed,_features[i]);
    }
#endif
    return isFieldContain;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > Contain::searchClosedField(Matrix<WIDTH, HEIGHT>&field,const Point& pos){
    std::queue<Point> work;
    Matrix<WIDTH, HEIGHT> _field = field;
    
    square closed_sq;
    
    work.push(pos);
    closed_sq.begin = pos; closed_sq.end = pos;
    
    while(!work.empty()){
        Point pos = work.front();
        
        closed_sq.begin.x = std::min(closed_sq.begin.x,pos.x);
        closed_sq.begin.y = std::min(closed_sq.begin.y,pos.y);
        closed_sq.end.x = std::max(closed_sq.end.x,pos.x);
        closed_sq.end.y = std::max(closed_sq.end.y,pos.y);
        
        CLOCKWISE_FOR(clock){
            Point search_point = pos + clock;
            if(search_point.x < 0 || search_point.x >= WIDTH ||
               search_point.y < 0 || search_point.y >= HEIGHT )continue;
            if(field[search_point.y][search_point.x] == false){
                work.push(search_point);
                field[search_point.y][search_point.x] = true;
            }
        }
        work.pop();
    }

    return xorField(trimField(field, closed_sq),trimField(_field, closed_sq));
}

bool Contain::Execution(const Field& field,std::size_t index){
    Field _field = field;
    square closed_sq;
    bool isAllFieldContainRemainBlock=true;
    
    for(int i=0;i<FIELD_HEIGHT;i++){
        for(int j=0;j<FIELD_WIDTH;j++){
            if(_field[i][j] == false){
                isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field,Point(j,i)),index);
                if(!isAllFieldContainRemainBlock)return false;
            }
        }
    }
    return isAllFieldContainRemainBlock;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
bool Contain::Execution(const Matrix<WIDTH,HEIGHT>& field,std::size_t index){
    Matrix<WIDTH,HEIGHT> _field = field;
    square closed_sq;
    bool isAllFieldContainRemainBlock=true;
    
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            if(_field[i][j] == false){
                isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field,Point(j,i)),index);
                if(!isAllFieldContainRemainBlock)return false;
            }
        }
    }
    return isAllFieldContainRemainBlock;
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

Contain::geometry_feature Contain::calcGeometryFeature(const std::vector< std::vector<int> >& block){
    std::vector< std::vector<int> > parted_block = particalBlock(block);
    geometry_feature gf;
    int initial_point,segment_size;
    
    if(block.size()==0)return gf;

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

Contain::geometry_feature Contain::calcGeometryFeatureField(const std::vector< std::vector<int> >& field){
    std::vector< std::vector<int> > parted_block = particalClosedField(field);
    geometry_feature gf;
    int initial_point,segment_size;
    
    for(int j=0; j<parted_block[0].size(); j++){
        initial_point=-1;
        segment_size=0;
        for(int i=0; i<parted_block.size(); i++){
            if(initial_point == -1 && parted_block[i][j] == 1)initial_point = i;
            if(initial_point != -1 && parted_block[i][j] == 0){segment_size = i - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 1)segment_size = std::max(segment_size ,i - initial_point + 1);
        }
        if(segment_size > 0)gf.first.push_back(segment_size);
    }
    
    for(int i=0; i<parted_block.size(); i++){
        initial_point=-1;
        segment_size=0;
        for(int j=0; j<parted_block[i].size(); j++){
            if(initial_point == -1 && parted_block[i][j] == 0)initial_point = j;
            if(initial_point != -1 && parted_block[i][j] == 1){segment_size = j - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 0)segment_size = std::max(segment_size ,j - initial_point + 1);
        }
        if(segment_size > 0)gf.second.push_back(segment_size);
    }
    
    if(gf.second.size() > gf.first.size())std::swap(gf.first,gf.second);
    
    std::sort(gf.first.begin(),gf.first.end(),std::greater<int>());
    std::sort(gf.second.begin(),gf.second.end(),std::greater<int>());
    
    return gf;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
Contain::geometry_feature Contain::calcGeometryFeatureField(const Matrix<WIDTH,HEIGHT>& field){
    std::vector< std::vector<int> > parted_block = particalClosedField(field);
    geometry_feature gf;
    int initial_point,segment_size;
    
    for(int j=0; j<parted_block[0].size(); j++){
        initial_point=-1;
        segment_size=0;
        for(int i=0; i<parted_block.size(); i++){
            if(initial_point == -1 && parted_block[i][j] == 0)initial_point = i;
            if(initial_point != -1 && parted_block[i][j] == 1){segment_size = i - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 0)segment_size = std::max(segment_size ,i - initial_point + 1);
        }
        if(segment_size > 0)gf.first.push_back(segment_size);
    }
    
    for(int i=0; i<parted_block.size(); i++){
        initial_point=-1;
        segment_size=0;
        for(int j=0; j<parted_block[i].size(); j++){
            if(initial_point == -1 && parted_block[i][j] == 0)initial_point = j;
            if(initial_point != -1 && parted_block[i][j] == 1){segment_size = j - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 0)segment_size = std::max(segment_size ,j - initial_point + 1);
        }
        if(segment_size > 0)gf.second.push_back(segment_size);
    }
    
    if(gf.second.size() > gf.first.size())std::swap(gf.first,gf.second);
    
    std::sort(gf.first.begin(),gf.first.end(),std::greater<int>());
    std::sort(gf.second.begin(),gf.second.end(),std::greater<int>());
    
    return gf;
}



bool Contain::isContain(const std::vector< std::vector<int> >& field,geometry_feature gf){
    geometry_feature root_gf = calcGeometryFeatureField(field);
    
    if(root_gf.first.size() < gf.first.size() || root_gf.second.size() < gf.second.size())return false;
    
    for(int i=0; i<gf.first.size(); i++){
        if(root_gf.first[i] < gf.first[i])return false;
    }
    for(int i=0; i<gf.second.size(); i++){
        if(root_gf.second[i] < gf.second[i])return false;
    }
    
    return true;
}

#endif

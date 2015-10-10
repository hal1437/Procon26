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
    typedef std::pair<std::vector<int>,std::vector<int>> geometry_feature;
    typedef std::pair<square, std::vector<std::vector<int>> > closed_range;
    
    std::vector<h_type*> heuristicses;
public:
    static constexpr bool isEnableReserve = false;
    static constexpr int  MAX_CLOSED_FIELD_SIZE = 20;
    
    bool Execution(const Field& field,const Transform& trans,std::size_t index,std::vector<int>& reserveBlocks);
    
    Contain(const Problem& prob);
    //Contain()=default;
    Contain(const Contain&)=default;
    
private:
    const Problem& _prob;
    std::vector<geometry_feature> _features;
    struct square{
        Point begin,end;
        bool operator==(const square& rhs){return this->begin == rhs.begin && this->end == rhs.end;}
    };
    
    closed_range searchClosedField(const Field& field,const Point& pos);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<std::vector<int>> trimField(const Matrix<WIDTH, HEIGHT>& field, const  square& sq);

    bool isFieldContainAllRemainBlocks(const Field& field,const std::vector<std::vector<int>>& closed,std::size_t index,std::vector<int> reserveBlocks);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block);
    
    geometry_feature calcGeometryFeature(const std::vector< std::vector<int> >& block);
    
    std::vector<Point> getReachable(const Field& field,const Block& block,const Transform& trans);
    
public:
    
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

bool Contain::isFieldContainAllRemainBlocks(const Field& field,const std::vector<std::vector<int>>& closed,std::size_t index,std::vector<int> reserveBlocks){
    bool isFieldContain=false;
    int N = _prob.Count();
#ifdef USE_PARALLEL
    PARALLEL_FOR(if(reserveBlocks[P_IT] < 0){
        if(isContain(closed,_features[P_IT])){
            isFieldContain = true;
            reserveBlocks[P_IT] = 1; //used
            break;
        }
    });
#else
    for(int i=index; i<N; i++){
        //std::cout << _prob.GetBlock(i) << std::endl;
        if(reserveBlocks[i] < 0){
            if(isContain(closed,_features[i])){
                isFieldContain = true;
                reserveBlocks[i] = 1; //used
                break;
            }
        }
    }
#endif
    return isFieldContain;
}

Contain::closed_range Contain::searchClosedField(const Field& field,const Point& pos){
    std::queue<Point> work;
    Field _field = field;
    square closed_sq;
    
    work.push(pos);
    closed_sq.begin = pos; closed_sq.end = pos;
    
    while(!work.empty()){
        if(work.size() > MAX_CLOSED_FIELD_SIZE){closed_sq.begin = Point(-1,-1); return std::make_pair(closed_sq, std::vector<std::vector<int>>());}
        Point pos = work.front();
        
        closed_sq.begin.x = std::min(closed_sq.begin.x,pos.x);
        closed_sq.begin.y = std::min(closed_sq.begin.y,pos.y);
        closed_sq.end.x = std::max(closed_sq.end.x,pos.x);
        closed_sq.end.y = std::max(closed_sq.end.y,pos.y);
        
        CLOCKWISE_FOR(clock){
            Point search_point = pos + clock;
            if(search_point.x < 0 || search_point.x >= FIELD_WIDTH ||
               search_point.y < 0 || search_point.y >= FIELD_HEIGHT )continue;
            if(_field[search_point.y][search_point.x] == false){
                work.push(search_point);
                _field[search_point.y][search_point.x] = true;
            }
        }
        work.pop();
    }
    
    return std::make_pair(closed_sq , xorField(trimField(field, closed_sq),trimField(_field, closed_sq)));
}

bool Contain::Execution(const Field& field,const Transform& trans, std::size_t index,std::vector<int>& reserveBlocks){
    Field _field = field;
    square closed_sq;
    std::vector<Point> reachable = getReachable(field | _prob.GetField(), _prob.GetBlock(index), trans);
    std::vector<square> tabu_list;

    bool isAllFieldContainRemainBlock=true;
    _field = static_cast<Field>(_field | _prob.GetField());
    
    for(Point& pos: reachable){
        //std::cout << _field << std::endl;
        auto closed = searchClosedField(_field,pos);
        if(closed.second.size() == 0 || std::find(tabu_list.begin(),tabu_list.end(),closed.first) != tabu_list.end() ) continue;
        tabu_list.push_back(closed.first);
        isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,closed.second,index,reserveBlocks);
        if(!isAllFieldContainRemainBlock)return false;
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

bool Contain::isContain(const std::vector< std::vector<int> >& field,geometry_feature gf){
    geometry_feature root_gf = calcGeometryFeature(field);
    
    if(root_gf.first.size() < gf.first.size() || root_gf.second.size() < gf.second.size())return false;
    
    for(int i=0; i<gf.first.size(); i++){
        if(root_gf.first[i] < gf.first[i])return false;
    }
    for(int i=0; i<gf.second.size(); i++){
        if(root_gf.second[i] < gf.second[i])return false;
    }
    
    return true;
}

std::vector<Point> Contain::getReachable(const Field& field,const Block& block,const Transform& trans){
    std::vector<Point> reachable;
    
    Transform setLarge(Point(1,1),Constants::ANGLE0,false);
    Matrix<BLOCK_WIDTH+2, BLOCK_HEIGHT+2> L_Block;
    
    L_Block.Projection(block,setLarge);
    L_Block = (L_Block.GetReachable()).GetRotate(trans.angle).GetReverse(trans.reverse);
    
    for(int i=0; i<BLOCK_HEIGHT+2; i++){
        for(int j=0; j<BLOCK_WIDTH+2; j++){
            Point onFieldPos = Point(j,i) + trans.pos - Point(1,1);
            if(onFieldPos.x >= 0 && onFieldPos.x < FIELD_WIDTH && onFieldPos.y >= 0 && onFieldPos.y < FIELD_HEIGHT){
                if(L_Block[i][j]==1 && field[onFieldPos.y][onFieldPos.x]==0)reachable.push_back(Point(j,i) + trans.pos - Point(1,1));
            }
        }
    }
    return reachable;
}

#endif

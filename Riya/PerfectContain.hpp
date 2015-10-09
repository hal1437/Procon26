
//
//  PerfectContain.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/09.
//  Copyright © 2015年 Riya.Liel. All rights reserved.
//

#ifndef PerfectContain_h
#define PerfectContain_h

#include"blockUtility.h"
#include"PARALLEL.hpp"
#include"../Hal/Perfect/PerfectComposit.h"
#include"Config.h"
#include<queue>
#include<algorithm>
#include<future>

class PerfectContain/* : public Perfect<Field,BlockLayer>*/{
private:
    struct square;
    typedef Perfect<Field,BlockLayer> h_type;
    typedef std::pair<std::vector<int>,std::vector<int>> geometry_feature;
    typedef std::pair<square, std::vector<std::vector<int>> > closed_range;
    
    std::vector<h_type*> heuristicses;
public:
    static constexpr bool isEnableReserve = true;
    static constexpr int  MAX_CLOSED_FIELD_SIZE = 10;
    
    bool Execution(Field& field,std::size_t index,std::vector<Transform>& reserve_trans);
    bool Execution(Field& field,std::size_t index);
    
    PerfectContain(const Problem& prob);
    //Contain()=default;
    PerfectContain(const PerfectContain&)=default;
    
    enum class CONTAIN_PARAMS{
        NO_CONTAIN=0,
        CONTAIN,
        PERFECT_CONTAIN
    };
    
private:
    const Problem& _prob;
    std::vector<geometry_feature> _features;
    struct square{
        Point begin,end;
    };
    
    closed_range searchClosedField(const Field& field,const Point& pos);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<std::vector<int>> trimField(const Matrix<WIDTH, HEIGHT>& field, const  square& sq);
    
    bool isFieldContainAllRemainBlocks(Field& field,const closed_range& closed,std::size_t index,std::vector<Transform>& reserve_trans);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block);
    
    geometry_feature calcGeometryFeature(const std::vector< std::vector<int> >& block);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeatureField(const Matrix<WIDTH,HEIGHT>& field);
    
    geometry_feature calcGeometryFeatureField(const std::vector< std::vector<int> >& field);
    
    CONTAIN_PARAMS isContain(const std::vector< std::vector<int> >& field,geometry_feature gf);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<Transform> GetListLayPossible(const std::vector< std::vector<int> >& _field,const Matrix<WIDTH,HEIGHT>& matrix, const std::vector< std::vector<int> >& _Mask);
    
    
};

PerfectContain::PerfectContain(const Problem& prob):_prob(prob){
    _features.resize(prob.Count());;
    
    for(std::size_t i=0; i<_prob.Count(); i++){
        _features[i] = calcGeometryFeature(particalBlock(_prob.GetBlock(i)));
    }
}

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector<std::vector<int>> PerfectContain::trimField(const Matrix<WIDTH, HEIGHT>& field, const square& sq){
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

bool PerfectContain::isFieldContainAllRemainBlocks(Field& field,const closed_range& closed,std::size_t index,std::vector<Transform>& reserve_trans){
    bool isFieldContain=false;
    int N = static_cast<int>(_prob.Count());
    
    if((closed.first.end.x - closed.first.begin.x) + (closed.first.end.y - closed.first.begin.y) > MAX_CLOSED_FIELD_SIZE)return true;
    
    for(int i=int(index); i<N; i++){
        if(reserve_trans[i].isEnable())continue;
        switch(isContain(closed.second,_features[i])){
            case CONTAIN_PARAMS::CONTAIN:
                isFieldContain = true;
                break;
            case CONTAIN_PARAMS::NO_CONTAIN:
                break;
            case CONTAIN_PARAMS::PERFECT_CONTAIN:
                isFieldContain = true;
                
                std::vector<Transform> hands = GetListLayPossible(closed.second, _prob.GetBlock(i), trimField(_prob.GetField(), closed.first));
                if(hands.size()!=0){
                    hands[0].pos = hands[0].pos + closed.first.begin;
                    reserve_trans[i] = hands[0];
                    field.Projection(_prob.GetBlock(i),hands[0]);
                }
                break;
        }
    }
    return isFieldContain;
}

PerfectContain::closed_range PerfectContain::searchClosedField(const Field& field,const Point& pos){
    std::queue<Point> work;
    Field _field = field;
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

bool PerfectContain::Execution(Field& field,std::size_t index,std::vector<Transform>& reserve_trans){
    Field _field = field;
    square closed_sq;
    bool isAllFieldContainRemainBlock=true;
    
    _field = static_cast<Field>(_field | _prob.GetField());
    
    for(int i=0;i<FIELD_HEIGHT;i++){
        for(int j=0;j<FIELD_WIDTH;j++){
            if(_field[i][j] == false){
                isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field ,Point(j,i)),index,reserve_trans);
                if(!isAllFieldContainRemainBlock)return false;
            }
        }
    }
    return isAllFieldContainRemainBlock;
}

bool PerfectContain::Execution(Field& field,std::size_t index){
    Field _field = field;
    square closed_sq;
    std::vector<Transform> reserve_trans(_prob.Count());
    bool isAllFieldContainRemainBlock=true;
    
    _field = static_cast<Field>(_field | _prob.GetField());
    
    for(int i=0;i<FIELD_HEIGHT;i++){
        for(int j=0;j<FIELD_WIDTH;j++){
            if(_field[i][j] == false){
                isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field ,Point(j,i)),index,reserve_trans);
                if(!isAllFieldContainRemainBlock)return false;
            }
        }
    }
    return isAllFieldContainRemainBlock;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
PerfectContain::geometry_feature PerfectContain::calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block){
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

PerfectContain::geometry_feature PerfectContain::calcGeometryFeature(const std::vector< std::vector<int> >& block){
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

PerfectContain::geometry_feature PerfectContain::calcGeometryFeatureField(const std::vector< std::vector<int> >& field){
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
PerfectContain::geometry_feature PerfectContain::calcGeometryFeatureField(const Matrix<WIDTH,HEIGHT>& field){
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

PerfectContain::CONTAIN_PARAMS PerfectContain::isContain(const std::vector< std::vector<int> >& field,geometry_feature gf){
    geometry_feature root_gf = calcGeometryFeature(field);
    bool isPerfect = true;
    
    if(root_gf.first.size() < gf.first.size() || root_gf.second.size() < gf.second.size())return CONTAIN_PARAMS::NO_CONTAIN;
    
    isPerfect &= root_gf.first.size() == gf.first.size() && root_gf.second.size() == gf.second.size();
    
    for(int i=0; i<gf.first.size(); i++){
        if(root_gf.first[i] != gf.first[i])isPerfect = false;
        if(root_gf.first[i] < gf.first[i])return CONTAIN_PARAMS::NO_CONTAIN;
    }
    for(int i=0; i<gf.second.size(); i++){
        if(root_gf.second[i] != gf.second[i])isPerfect = false;
        if(root_gf.second[i] < gf.second[i])return CONTAIN_PARAMS::NO_CONTAIN;
    }
    
    return isPerfect ? CONTAIN_PARAMS::PERFECT_CONTAIN : CONTAIN_PARAMS::CONTAIN;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector<Transform> PerfectContain::GetListLayPossible(const std::vector< std::vector<int> >& _field,const Matrix<WIDTH,HEIGHT>& matrix, const std::vector< std::vector<int> >& _Mask){
    Matrix<WIDTH,HEIGHT> sample[2][4];
    std::map< Matrix<WIDTH,HEIGHT>,struct Transform > map;
    typedef Matrix<WIDTH,HEIGHT> current;

    Matrix<WIDTH,HEIGHT> sample_base(matrix),field,Mask;
    field = ~field;
    
    for(int i=0; i<_field.size(); i++){
        for(int j=0; j<_field[i].size(); j++){
            field[i][j] = _field[i][j] ? 0 : 1;
            Mask[i][j] = _Mask[i][j];
        }
    }
    
    for(int i=0;i<2;i++){
        sample_base.Reverse(i);
        for(int j=0;j<4;j++){
            sample[i][j].Projection(sample_base);
            sample_base.Rotate(Constants::ANGLE90);
        }
    }
    std::vector<class Transform> answer;
    for(int i = -8;i < static_cast<int>(_field.size());i++){
        for(int j = -8;j < static_cast<int>(_field[0].size()); j++){
            for(int r=0;r<2;r++){
                for(int k=0;k<4;k++){
                    Transform::Transform move_trans(Point(j,i),Constants::ANGLE0,false);
                    if(field.ProjectionTest(sample[r][k],move_trans,Mask)){
                        struct Transform t(Point(j,i),static_cast<Constants::ANGLE>(k*90),r);
                        map.insert(std::make_pair(current(field).Projection(sample[r][k],move_trans),t));
                    }
                }
            }
        }
    }
    for(auto x:map)answer.push_back(x.second);
    return answer;
}


#endif /* PerfectContain_h */

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
#include"../Utility/PARALLEL.hpp"
#include"../Hal/Perfect/PerfectComposit.h"
#include"Config.h"
#include<queue>
#include<algorithm>
#include<future>
#include"petternMatching.h"

class PerfectContain/* : public Perfect<Field,BlockLayer>*/{
private:
    struct square;

    typedef std::pair<std::vector<int>,std::vector<int>>        geometry_feature;
    typedef std::pair<square, std::vector<std::vector<int>> >   closed_range;
    
public:
    static constexpr bool isEnableReserve = true;
    static constexpr int  MAX_CLOSED_FIELD_SIZE = 10;
    
    bool Execution(Field& field, const Transform& trans,std::size_t index,std::vector<Transform>& reserve_trans);
    bool Execution(Field& field, const Transform& trans,std::size_t index);
    
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
    PetternSolver closed_solver = PetternSolver();
    
    std::vector<geometry_feature> _features;
    std::vector< std::pair<Block,std::size_t> > min_closed_fix_blocks;
    
    struct square{
        Point begin,end;
        bool operator==(const square& rhs){return this->begin == rhs.begin && this->end == rhs.end;}
    };
    
    closed_range searchClosedField(const Field& field,const Point& pos)const;
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<std::vector<int>> trimField(const Matrix<WIDTH, HEIGHT>& field, const  square& sq)const;
    
    bool isFieldContainAllRemainBlocks(Field& field,const closed_range& closed,std::size_t index,std::vector<Transform>& reserve_trans);
    
    bool solveSubproblem(const Field& field,const closed_range& closed,const std::vector< std::pair< Block*, bool > >& block_list, const std::vector<int>& solve_index,std::vector<Transform>& ans,int i=0);
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    geometry_feature calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block)const;
    geometry_feature calcGeometryFeature(const std::vector< std::vector<int> >& block)const;
    
    CONTAIN_PARAMS isContain(const std::vector< std::vector<int> >& field,geometry_feature gf)const;
    
    template<std::size_t WIDTH,std::size_t HEIGHT>
    std::vector<Transform> GetListLayPossible(const std::vector< std::vector<int> >& _field,const Matrix<WIDTH,HEIGHT>& matrix, const std::vector< std::vector<int> >& _Mask)const;
    
    std::vector<Point> getReachable(const Field& field,const Block& block,const Transform& trans);
    
    
};

PerfectContain::PerfectContain(const Problem& prob):_prob(prob){
    _features.resize(prob.Count());
    min_closed_fix_blocks.resize(prob.Count());
    std::fill(min_closed_fix_blocks.begin(),min_closed_fix_blocks.end(),std::make_pair(solve_field(),-1));
    
    for(std::size_t i=0; i<_prob.Count(); i++){
        _features[i] = calcGeometryFeature(particalBlock(_prob.GetBlock(i)));
        
        auto parted_block = particalBlock(_prob.GetBlock(i));
        if(parted_block.size() < PETTERN_MATCH_MAX_HEIGHT && parted_block[0].size() < PETTERN_MATCH_MAX_WIDTH){
            min_closed_fix_blocks[i] = std::make_pair(_prob.GetBlock(i), i) ;
        }
    }
}

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector<std::vector<int>> PerfectContain::trimField(const Matrix<WIDTH, HEIGHT>& field, const square& sq)const{
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
    std::vector<Transform> hands;
    
    if(closed.first.end.y - closed.first.begin.y < PETTERN_MATCH_MAX_HEIGHT && closed.first.end.x - closed.first.begin.x < PETTERN_MATCH_MAX_WIDTH){
        std::vector<int> solve_index;
        solve_field subproblem;
        subproblem = ~subproblem;

        for(int i=0; i<closed.second.size(); i++){
            for(int j=0; j<closed.second[i].size(); j++){
                subproblem[i][j] = closed.second[i][j] ? 0 : 1;
            }
        }
        
        std::vector< std::pair< Block*, bool > > block_list;
        std::vector< int > indexes;
        
        for(int i=index; i<_prob.Count(); i++){
            if(min_closed_fix_blocks[i].second != -1 && !reserve_trans[i].isEnable()){
                block_list.push_back(std::make_pair( &min_closed_fix_blocks[i].first , true ));
                indexes.push_back(min_closed_fix_blocks[i].second);
            }
        }
        if(closed_solver.isMatched(std::hash<solve_field>()(subproblem), block_list, solve_index)){
            if(solveSubproblem(field,closed,block_list,solve_index,hands)){
                for(int i=0; i<solve_index.size(); i++){
                    reserve_trans[indexes[solve_index[i]]] = hands[solve_index[i]];
                    field.Projection(_prob.GetBlock(indexes[solve_index[i]]),hands[solve_index[i]]);
                }
                return true;
            }
        }
        else return false;
    }
    
#ifdef USE_PARALLEL
    PARALLEL_FOR(index, N, if(reserve_trans[P_IT].isEnable())continue;
                 switch(isContain(closed.second,_features[P_IT])){
                     case CONTAIN_PARAMS::CONTAIN:
                         isFieldContain = true;
                         break;
                     case CONTAIN_PARAMS::NO_CONTAIN:
                         break;
                     case CONTAIN_PARAMS::PERFECT_CONTAIN:
                         isFieldContain = true;
                         
                         std::vector<Transform> hands = GetListLayPossible(closed.second, _prob.GetBlock(P_IT), trimField(_prob.GetField(), closed.first));
                         if(hands.size()!=0){
                             hands[0].pos = hands[0].pos + closed.first.begin;
                             reserve_trans[P_IT] = hands[0];
                             field.Projection(_prob.GetBlock(P_IT),hands[0]);
                         }
                         P_IT = N; break;
                 })
#else
    for(int i=int(index); i<N; i++){
        if(reserve_trans[i].isEnable())continue;
        switch(isContain(closed.second,_features[i])){
            case CONTAIN_PARAMS::CONTAIN:
                isFieldContain = true;
                break;
            case CONTAIN_PARAMS::NO_CONTAIN:
                break;
            case CONTAIN_PARAMS::PERFECT_CONTAIN:
                std::vector<Transform> hands = GetListLayPossible(closed.second, _prob.GetBlock(i), trimField(_prob.GetField(), closed.first));
                if(hands.size()!=0){
                    hands[0].pos = hands[0].pos + closed.first.begin;
                    reserve_trans[i] = hands[0];
                    field.Projection(_prob.GetBlock(i),hands[0]);
                    return true;
                }
                else isFieldContain=false;
                break;
        }
    }
#endif
    return isFieldContain;
}

bool PerfectContain::solveSubproblem(const Field& field,const closed_range& closed,const std::vector< std::pair< Block*, bool > >& block_list, const std::vector<int>& solve_index,std::vector<Transform>& ans,int i){
    Block block;
    ans.resize(block_list.size());
    Field _field = field | _prob.GetField();
    bool isAllFilled=true;

    for(int j=closed.first.begin.y; j<=closed.first.end.y; j++){
        for(int k=closed.first.begin.x; k<=closed.first.end.x; k++){
            isAllFilled &= field[j][k]==1;
        }
    }
    
    if(isAllFilled)return true;
    if(i >= solve_index.size())return false;
    
    block = (*block_list[solve_index[i]].first);

    auto hands = GetListLayPossible(closed.second, block, trimField(_field , closed.first));
    for(auto& hand: hands){
        hand.pos = hand.pos + closed.first.begin;
        //std::cout << _field.GetProjection(block,hand) << std::endl;
        //std::cout << block.GetTransform(hand) << std::endl;
        if(solveSubproblem(_field.GetProjection(block,hand),closed,block_list,solve_index,ans,i+1)){
            ans[solve_index[i]] = hand;
            return true;
        }
    }

    return false;
}


PerfectContain::closed_range PerfectContain::searchClosedField(const Field& field,const Point& pos)const{
    std::queue<Point> work;
    Field _field = field | _prob.GetField();
    square closed_sq;
    
    if(_field[pos.y][pos.x])return std::make_pair(closed_sq, std::vector<std::vector<int>>());
    else _field[pos.y][pos.x] = true;
    
    work.push(pos);
    closed_sq.begin = pos; closed_sq.end = pos;
    
    while(!work.empty()){
        if((closed_sq.end.x - closed_sq.begin.x) * (closed_sq.end.y - closed_sq.begin.y) > MAX_CLOSED_FIELD_SIZE){
            closed_sq.begin = Point(-1,-1); return std::make_pair(closed_sq, std::vector<std::vector<int>>());
        }
        
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
    
    return std::make_pair(closed_sq , xorField(trimField(static_cast<Field>(field | _prob.GetField()), closed_sq),trimField(_field, closed_sq)));
}

bool PerfectContain::Execution(Field& field,const Transform& trans, std::size_t index,std::vector<Transform>& reserve_trans){
    square closed_sq;
    //std::cout << _prob.GetBlock(index) << std::endl;
    std::vector<Point> reachable = getReachable(static_cast<Field>(field | _prob.GetField()), _prob.GetBlock(index), trans);
    std::vector<square> tabu_list;
    bool isAllFieldContainRemainBlock=true;
    
#ifdef USE_PARALLEL
    PARALLEL_FOR(0,reachable.size(),auto closed = searchClosedField(_field ,reachable[P_IT]);
                 if(closed.second.size() != 0)isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field ,reachable[P_IT]),index,reserve_trans);
                 else continue;
                 if(!isAllFieldContainRemainBlock)break;)
#else
    for(Point& pos: reachable){
        auto closed = searchClosedField(field ,pos);
        if(closed.second.size() == 0 || std::find(tabu_list.begin(),tabu_list.end(),closed.first) != tabu_list.end() ) continue;
        tabu_list.push_back(closed.first);
        isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,closed,index,reserve_trans);
    }
#endif
    return isAllFieldContainRemainBlock;
}

bool PerfectContain::Execution(Field& field,const Transform& trans,std::size_t index){
    square closed_sq;
    std::vector<Transform> reserve_trans(_prob.Count());
    std::vector<square> tabu_list;
    std::vector<Point> reachable = getReachable(field | _prob.GetField(), _prob.GetBlock(index), trans);
    
    bool isAllFieldContainRemainBlock=true;
    
#ifdef USE_PARALLEL
    PARALLEL_FOR(0,reachable.size(),auto closed = searchClosedField(_field ,reachable[P_IT]);
                 if(closed.second.size() != 0)isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,searchClosedField(_field ,reachable[P_IT]),index,reserve_trans);
                 else continue;
                 if(!isAllFieldContainRemainBlock)break;)
#else
    for(Point& pos: reachable){
        //std::cout << _field << std::endl;
        auto closed = searchClosedField(field,pos);
        if(closed.second.size() == 0 || std::find(tabu_list.begin(),tabu_list.end(),closed.first) != tabu_list.end() ) continue;
        tabu_list.push_back(closed.first);
        isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,closed,index,reserve_trans);
        if(!isAllFieldContainRemainBlock)return false;
    }
#endif
    return isAllFieldContainRemainBlock;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
PerfectContain::geometry_feature PerfectContain::calcGeometryFeature(const Matrix<WIDTH,HEIGHT>& block)const{
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

PerfectContain::geometry_feature PerfectContain::calcGeometryFeature(const std::vector< std::vector<int> >& block)const{
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


PerfectContain::CONTAIN_PARAMS PerfectContain::isContain(const std::vector< std::vector<int> >& field,geometry_feature gf)const{
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
std::vector<Transform> PerfectContain::GetListLayPossible(const std::vector< std::vector<int> >& _field,const Matrix<WIDTH,HEIGHT>& matrix, const std::vector< std::vector<int> >& _Mask)const{
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

std::vector<Point> PerfectContain::getReachable(const Field& field,const Block& block,const Transform& trans){
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

#endif /* PerfectContain_h */

//
//  AnswerHistory.h
//  Procon26
//
//  Created by Riya.Liel on 2015/06/14.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_AnswerHistory_h
#define Procon26_AnswerHistory_h

#include<vector>
#include<utility>
#include"../Structure/Matrix.hpp"
#include"../Structure/Answer.h"

template<class TRANSFORM,class BLOCK>
class Answer_history{
public:
    explicit Answer_history(Problem& problem){ans_list.resize(problem.Count()); eval_list.resize(problem.Count());}
    Answer_history()=default;
    
    void set_ans(std::pair<TRANSFORM, BLOCK> &&node,int i){ans_list[i] = node;}
    void set_ans(std::pair<TRANSFORM, BLOCK> &node,int i){ans_list[i] = node;}
    
    void set_eval(long eval,int i){eval_list[i] = std::make_pair(eval,i);}
    long get_eval(int i){return eval_list[i].first;};
    
    const std::pair<TRANSFORM,BLOCK>& get_ans(int i){return ans_list[i];}
    
    std::size_t search_bad_index();
    
    std::pair<TRANSFORM,BLOCK>& operator[](int index){return ans_list[index];}
    long size(){return ans_list.size();}

    void returnTheHand(std::size_t index,Field& field){
        long size = ans_list.size();
        for(long i=size-1;i>=static_cast<long>(index);i--){
            field.ReverseProjection(ans_list[i].second,ans_list[i].first);
        }
    }
    Answer TranslateAnswer(Problem& problem){
        Answer answer(problem);
        for(int i=0;i<ans_list.size();i++){
            if(ans_list[i].first.isEnable()){
                answer.SetBlock(i,ans_list[i].second);
                answer.SetTransform(i, ans_list[i].first);
            }
            else answer.SetTransform(i,Transform());
        }
        return answer;
    }
private:
    std::vector< std::pair<TRANSFORM,BLOCK> > ans_list;
    std::vector< std::pair<long,std::size_t> > eval_list;
};

template<class TRANSFORM,class BLOCK>
std::size_t Answer_history<TRANSFORM,BLOCK>::search_bad_index(){
    int i;
    bool isPathed=false;;
    
    /*
    auto it = std::min_element(ans_list.begin(),ans_list.end());
    std::size_t worst_index = it - ans_list.begin();
    bool upward=false;
    long i,old_eval = -1;
    
    for(i=worst_index-1; i>=0; i--){
        if(!upward){
            if(old_eval < eval_list[i].first){
                upward = true;
            }
        }
        else{
            if(old_eval > eval_list[i].first){
                break;
            }
        }
    }
     */
    
    for(i=ans_list.size()-1; i>=0; i--){
        if(i==0)break;
        if(isPathed && ans_list[i].first.isEnable())break;
        if(!ans_list[i].first.isEnable()){
            isPathed = true;
            continue;
        }
    }
    
    return i;
}

#endif

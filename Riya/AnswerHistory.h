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
    void set_eval(long eval,int i){eval_list[i] = eval;}
    long get_eval(int i){return eval_list[i];};
    std::pair<TRANSFORM,BLOCK>& operator[](int index){return ans_list[index];}
    long size(){return ans_list.size();}

    void returnTheHand(int index,Field& field){
        long size = ans_list.size();
        std::cout << field << std::endl;
        for(long i=size-1;i>=index;i--){
            field.ReverseProjection(ans_list[i].second, ans_list[i].first);
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
    std::vector<long> eval_list;
};

#endif

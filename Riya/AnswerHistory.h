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
    
    void push_back(std::pair<TRANSFORM, BLOCK> node){ans_list.push_back(node);};
    std::pair<TRANSFORM,BLOCK>& operator[](int index){return ans_list[index];}
    int size(){return ans_list.size();}
    
    void returnTheHand(int index,Field& field){
        int size = ans_list.size();
        for(int i=0;i<size-index;i++){
            field.ReverseProjection(ans_list.back().second, ans_list.back().first);
            ans_list.pop_back();
        }
    }
    
    Answer TranslateAnswer(Problem& problem){
        Answer answer(problem);
        for(int i=0;i<ans_list.size();i++){
            if(ans_list[i].first.isEnable())
                answer.AddBlocks(ans_list[i].first);
            else answer.AddBlocks();
        }
        return answer;
    }
    
private:
    std::vector< std::pair<TRANSFORM,BLOCK> > ans_list;
};

#endif

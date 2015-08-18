//
//  Board.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/08/10.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26_project__Board__
#define __Procon26_project__Board__

#include"GameTree.h"
#include"../AnswerHistory.h"
#include"../../Structure/Transform.h"
#include"../../Structure/Matrix.hpp"

class Board{
public:
    int entropy;
    Field field;
    Board(GameTreeNode* node);
    Board(Field _field);
    Board() = default;
    
    unsigned int calcBound();
    Answer getPathlist();
    Field constructField(Field,GameTreeNode*);
private:
    Answer_history<Transform, Block> pathlist;
};

Board::Board(Field _field){
    field = _field;
}

Board::Board(GameTreeNode* node){
    Problem problem(SOLVE_PROBREM);
    field = constructField(problem.GetField(), node);
}

Field Board::constructField(Field field , GameTreeNode* node){
    field.Projection(node->hand.second, node->hand.first);
    if(node->parent == nullptr)return field;
    return constructField(field, node->parent);
}

#endif /* defined(__Procon26_project__Board__) */

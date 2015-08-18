//
//  block_heuristics_solver.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/08/10.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26_project__block_heuristics_solver__
#define __Procon26_project__block_heuristics_solver__

#include<algorithm>
#include"Structure/GameTree.h"
#include"Structure/Board.h"
#include"../Structure/Answer.h"

bool FirstBoardEval(Board);
bool SecondBoardEval(Board);
void releaseNode(GameTreeNode*);

class Block_HS{
public:
    Answer solveAnswer();
private:
    int clacEvalution(Answer);
};

Answer Block_HS::solveAnswer(){
    Answer ans;
    std::vector<Answer> ans_list;
    Problem problem("quest1.txt");
    int depth=0;
    
    std::vector<Transform> hands;
    Field field;
    Board board;
    GameTreeNode* root = new GameTreeNode;
    GameTreeNode* back = root;
    root->path_penalty = 1e5; //infinity
    root->parent = nullptr;
    
    while(back != nullptr){
        board = Board(back);
        field = board.field;
        if(field.count() == FIELD_HEIGHT*FIELD_WIDTH){
            ans_list.push_back(board.getPathlist());
        }
        
        hands = field.GetListLayPossible(problem.GetBlock(depth), problem.GetField());
        for(Transform& hand: hands){
            Field new_field = field.GetProjection(problem.GetBlock(depth), hand);
            board = Board(new_field);
            if(FirstBoardEval(board)){
                GameTreeNode* new_node = new GameTreeNode;
                new_node->path_penalty= board.calcBound();
                if(back->path_penalty > new_node->path_penalty){
                    if(SecondBoardEval(board)){
                        back->children.push_back(new_node);
                    }
                }
            }
            depth++;
        }
        if(back->children.size() == 0)releaseNode(back);
    }
    
    return ans;
}

bool FirstBoardEval(Board){
    
    return 1;
}
bool SecondBoardEval(Board){
    
    return 1;
}

void releaseNode(GameTreeNode* node){
    
    if(node->parent->children.size() == 1)releaseNode(node->parent);
    
    node->parent->children.erase(node->parent->children.begin() ,
                                 std::remove_if(node->parent->children.begin(),
                                                node->parent->children.end(),
                                                [=](GameTreeNode* _node){return _node==node;}
                                                )
                                 );
    
    delete node;
}



#endif /* defined(__Procon26_project__block_heuristics_solver__) */

//
//  GameTree.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/08/10.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef __Procon26_project__GameTree__
#define __Procon26_project__GameTree__

#include<vector>
#include"../Parameters.h"
#include"../../Structure/Transform.h"
#include"../../Structure/Matrix.hpp"

struct GameTreeNode{
    int path_penalty,depth;
    GameTreeNode* parent;
    std::pair<Transform,Block> hand;
    std::vector<GameTreeNode*> children;
};

#endif /* defined(__Procon26_project__GameTree__) */

//
//  petternMatching.h
//  Procon26
//
//  Created by Riya.Liel on 2015/06/09.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_petternMatching_h
#define Procon26_petternMatching_h

#include<vector>
#include<array>
#include<utility>
#include"../Structure/Point.h"
#include"../Structure/Matrix.hpp"

#define PETTERN_MATCH_MAX_WIDTH 3
#define PETTERN_MATCH_MAX_HEIGHT 3

using block_hash = size_t;
//using block_list = std::array< block_hash , 512 >;
using solve_field = Matrix< PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>;
using petternTable = std::array< std::pair<block_hash , int> , 512>;

void solveSubproblem(solve_field subproblem, petternTable& table);
std::vector< std::vector<int> > particalProblem(std::vector< std::vector<int> > table);
std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino);
std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino,Point target);

petternTable solvePettern(){
    petternTable table;
    solve_field problem;
    std::pair<block_hash , int> dummy(-1,-1);
    std::fill(table.begin(),table.end(),dummy);
    solveSubproblem(problem, table);
    return table;
}

void solveSubproblem(solve_field subproblem, petternTable& table){
    std::vector< Point > reachable;
    solve_field polyomino=subproblem;
    size_t problem_hash = std::hash<Matrix<PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>>()(subproblem);
    
    if(subproblem.count() == PETTERN_MATCH_MAX_HEIGHT * PETTERN_MATCH_MAX_WIDTH)return;
    if(table[problem_hash].first!=-1)return;
    
    for(int i=0;i<PETTERN_MATCH_MAX_HEIGHT;i++){
        for(int j=0;j<PETTERN_MATCH_MAX_WIDTH;j++){
            if(subproblem[i][j] == 0){
                reachable.push_back(Point(j,i));
                goto break_2;
            }
        }
    }
    
break_2:
    
    while(!reachable.empty()){
        Point target = reachable.back();
        reachable.pop_back();
        polyomino[target.y][target.x]=1;
        subproblem[target.y][target.x]=1;
        
        solveSubproblem( subproblem , table );
        table[problem_hash] = std::make_pair(std::hash<solve_field>()(polyomino), std::hash<solve_field>()(subproblem));
        
        auto tmp = getReachable(subproblem, polyomino, target);
        reachable.insert(reachable.end(),tmp.begin(),tmp.end());
    }
    
}

std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino){
    std::vector< Point > reachable;
    return reachable;
}

std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino,Point target){
    std::vector< Point > reachable;
    
    if(target.x != PETTERN_MATCH_MAX_WIDTH-1 &&
       subproblem[target.y][target.x+1] == 0 && polyomino[target.y][target.x+1] == 0 ){
        reachable.push_back(Point(target.x+1,target.y));
    }
    
    if(target.x != 0 &&
       subproblem[target.y][target.x-1] == 0 && polyomino[target.y][target.x-1] == 0 ){
        reachable.push_back(Point(target.x-1,target.y));
    }
    
    if(target.y != PETTERN_MATCH_MAX_HEIGHT-1 &&
       subproblem[target.y+1][target.x] == 0 && polyomino[target.y+1][target.x] == 0 ){
        reachable.push_back(Point(target.x,target.y+1));
    }
    
    if(target.y != 0 &&
       subproblem[target.y-1][target.x] == 0 && polyomino[target.y-1][target.x] == 0 ){
        reachable.push_back(Point(target.x,target.y-1));
    }
    
    return reachable;
}

/*
std::vector< std::vector<int> > particalProblem(std::vector< std::vector<int> > table){
    for(int i=0; i<table.size(); i++){
        for(int j=0; j<table[i].size(); j++){
            if(table[i][j] == 0){
                break;
            }
            if(j==table[i].size()-1 && (i==0 || i==table.size()-1)){
                table.erase(table.begin()+i);
                i--;
                break;
            }
        }
    }
    
    for(int j=0; j<table[0].size(); j++){
        for(int i=0; i<table.size(); i++){
            if(table[i][j] == 0){
                if(j != table[0].size()-1)j = table[0].size()-2;
                break;
            }
            if(i==table[i].size()-1 && (j==0 || j==table[0].size()-1)){
                for(int k=0; k<table.size(); k++){
                    table[k].erase(table[k].begin());
                    j--;
                }
            }
        }
    }
    return table;
}
*/

#endif

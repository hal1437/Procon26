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
#include"../Structure/Point.h"

using block_hash = int;
using block_list = std::vector< block_hash >;
using field = std::vector< std::vector< int > >;
using petternTable = std::map< int, std::map< int,int > >;

void solveSubproblem(std::vector< std::vector<int> > subproblem, petternTable& table);
std::vector< std::vector<int> > particalProblem(std::vector< std::vector<int> > table);
std::vector< Point > getReachable(const std::vector< std::vector<int> > subproblem,field polyomino);
std::vector< Point > getReachable(const std::vector< std::vector<int> > subproblem,field polyomino,Point target);
int convertHash(field& block){return 0;}

petternTable solvePettern(std::vector< std::vector<int> > problem){
    petternTable table;
    solveSubproblem(problem, table);
    return table;
}

void solveSubproblem(std::vector< std::vector<int> > subproblem, petternTable& table){
    std::vector< Point > reachable;
    field polyomino=subproblem;
    int problem_hash = convertHash(subproblem);
    
    if(subproblem.empty())return;
    if(table.end() == table.find(problem_hash))return;
    
    std::fill(polyomino.front().begin(),polyomino.back().end(),0);
    
    for(int i=0;i<subproblem.size();i++){
        for(int j=0;j<subproblem[i].size();j++){
            if(subproblem[i][j] == 0){
                reachable.push_back(Point(i,j));
                goto break_2;
            }
        }
    }
    
break_2:
    
    while(!reachable.empty()){
        Point target = reachable.front();
        polyomino[target.y][target.x]=1;
        subproblem[target.y][target.x]=1;
        
        solveSubproblem( particalProblem(subproblem), table );
        //table[problem_hash].insert(convertHash(polyomino),convertHash(subproblem));
        
        auto tmp = getReachable(subproblem, polyomino, target);
        reachable.insert(reachable.end(),tmp.begin(),tmp.end());
    }
    
}

std::vector< Point > getReachable(const std::vector< std::vector<int> > subproblem,field polyomino){
    std::vector< Point > reachable;
    return reachable;
}

std::vector< Point > getReachable(const std::vector< std::vector<int> > subproblem,field polyomino,Point target){
    std::vector< Point > reachable;
    
    if(target.x != subproblem[target.y].size()-1 &&
       subproblem[target.y][target.x+1] == 0 && polyomino[target.y][target.x+1] == 0 ){
        reachable.push_back(Point(target.x+1,target.y));
    }
    
    if(target.x != 0 &&
       subproblem[target.y][target.x-1] == 0 && polyomino[target.y][target.x-1] == 0 ){
        reachable.push_back(Point(target.x-1,target.y));
    }
    
    if(target.y != subproblem.size()-1 &&
       subproblem[target.y+1][target.x] == 0 && polyomino[target.y+1][target.x] == 0 ){
        reachable.push_back(Point(target.x,target.y+1));
    }
    
    if(target.y != 0 &&
       subproblem[target.y-1][target.x] == 0 && polyomino[target.y-1][target.x] == 0 ){
        reachable.push_back(Point(target.x,target.y-1));
    }
    
    return reachable;
}

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

#endif

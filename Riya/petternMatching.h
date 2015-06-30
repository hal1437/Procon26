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
#include<queue>
#include<array>
#include<utility>
#include"../Structure/Point.h"
#include"../Structure/Matrix.hpp"
#include"../Structure/Array.hpp"

#define PETTERN_MATCH_MAX_WIDTH 3
#define PETTERN_MATCH_MAX_HEIGHT 3

#define CONSTEXPR_RIYA

using block_hash = size_t;
//using block_list = std::array< block_hash , 512 >;
using solve_field = Matrix< PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>;
using petternTable = Anct::Array< std::vector< std::pair<block_hash , size_t> >, 512>;

/*
void solveSubproblem(solve_field subproblem, petternTable& table);
//std::vector< std::vector<int> > particalProblem(std::vector< std::vector<int> > table);
static std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino);
static std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino,Point target);
static solve_field addPointToMatrix(solve_field source,Point pos);
static std::vector< solve_field > getPolyominoList(solve_field& polyomino,std::vector<Point> reachable);
static solve_field fieldNormalize(const solve_field& source);
CONSTEXPR_RIYA petternTable solve_pettern();
 */

class PetternSolver{
public:
    
    //static CONSTEXPR_RIYA int MAX_WIDTH=3;
    //static CONSTEXPR_RIYA int MAH_HEIGHT=3;
    //static CONSTEXPR_RIYA petternTable _Table = solvePettern();
    
    bool isMatched(petternTable& table,size_t subproblem_hash,std::vector<solve_field> block_list);
    
private:
    std::vector<solve_field> removeElement(std::vector<solve_field> list,int i);
    
    CONSTEXPR_RIYA petternTable solvePettern();
    CONSTEXPR_RIYA void solveSubproblem(solve_field subproblem, petternTable& table);
    CONSTEXPR_RIYA std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino);
    CONSTEXPR_RIYA std::vector< Point > getReachable(const solve_field subproblem,solve_field polyomino,Point target);
    CONSTEXPR_RIYA solve_field addPointToMatrix(solve_field source,Point pos);
    CONSTEXPR_RIYA std::vector< solve_field > getPolyominoList(solve_field& polyomino,std::vector<Point> reachable);
};

std::vector<solve_field> PetternSolver::removeElement(std::vector<solve_field> list,int i){
    list.erase(list.begin() + i);
    return list;
}

bool PetternSolver::isMatched(petternTable& table,size_t subproblem_hash,std::vector<solve_field> block_list){
    std::cout << "---subproblem---" << std::endl;
    std::cout << solve_field(subproblem_hash) << std::endl;
    if(subproblem_hash == pow(2,PETTERN_MATCH_MAX_WIDTH*PETTERN_MATCH_MAX_HEIGHT)-1)return true;
    for(int i=0;i<block_list.size();i++){
        auto find_target = table[subproblem_hash];
        auto itr = std::find_if(find_target.begin(),find_target.end(),
                             [&](const std::pair<block_hash , size_t>& elem){
                                 return elem.first == std::hash<solve_field>()(block_list[i]);
                             }
                             );
        if(itr != find_target.end()){
            std::cout << block_list[i] << std::endl;
            std::cout << solve_field(itr->first) << std::endl;
            if(isMatched(table,(*itr).second,removeElement(block_list, i)))return true;
        }
    }
    return false;
}

/*
bool petternMatch(petternTable& table,size_t solve_hash, std::vector<Matrix<PETTERN_MATCH_MAX_WIDTH,PETTERN_MATCH_MAX_HEIGHT>> list){
    bool is_can_solve = isMatched(table, solve_hash, list);
    return is_can_solve;
}
 */

CONSTEXPR_RIYA petternTable PetternSolver::solvePettern(){
    petternTable table;
    solve_field problem;
    //std::pair<block_hash , int> dummy(0,-1);
    //std::fill(table.begin(),table.end(),dummy);
    solveSubproblem(problem, table);
    return table;
}

CONSTEXPR_RIYA void PetternSolver::solveSubproblem(solve_field subproblem, petternTable& table){
    std::vector< Point > reachable;
    std::queue < solve_field > polyomino_queue;
    solve_field polyomino;
    size_t problem_hash = std::hash<Matrix<PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>>()(subproblem);
    auto source = subproblem;
    
    if(subproblem.count() == PETTERN_MATCH_MAX_HEIGHT * PETTERN_MATCH_MAX_WIDTH)return;
    if(table[problem_hash].size()!=0)return;
    /*
    table[problem_hash].push_back(std::make_pair(problem_hash,
                                                 std::hash<solve_field>()(solve_field())
                                                 )
                                  );
     */
    
    for(int i=0;i<PETTERN_MATCH_MAX_HEIGHT;i++){
        for(int j=0;j<PETTERN_MATCH_MAX_WIDTH;j++){
            if(subproblem[i][j] == 0){
                reachable.push_back(Point(j,i));
                goto break_2;
            }
        }
    }
    
break_2:
    
    if(reachable.empty())return;
    polyomino_queue.push( addPointToMatrix(solve_field(),reachable.back()) );
    
    while(!polyomino_queue.empty()){
        polyomino = polyomino_queue.front();
        polyomino_queue.pop();
        //subproblem[target.y][target.x]=1;
        
        std::cout << "-- solve problem --" << std::endl;
        std::cout << source << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << polyomino << std::endl;
        std::cout << subproblem << std::endl;
        std::cout << "-----" << std::endl;
        
        solveSubproblem( subproblem | polyomino , table );
        table[problem_hash].push_back(std::make_pair(std::hash<solve_field>()(polyomino),
                                                     std::hash<solve_field>()(subproblem | polyomino)
                                                     )
                                      );
        
        reachable = getReachable(source, polyomino);
        auto poly_list = getPolyominoList(polyomino, reachable);
        for(auto _polyomino : poly_list){
            polyomino_queue.push(_polyomino);
        }
    }
}

CONSTEXPR_RIYA solve_field PetternSolver::addPointToMatrix(solve_field source,Point pos){
    source[pos.y][pos.x] = 1;
    return source;
}

CONSTEXPR_RIYA std::vector< solve_field > PetternSolver::getPolyominoList(solve_field& polyomino,std::vector<Point> reachable){
    std::vector< solve_field > list;
    
    for(Point& pos: reachable){
        list.push_back( addPointToMatrix(polyomino, pos) );
    }
    
    return list;
}

CONSTEXPR_RIYA std::vector< Point > PetternSolver::getReachable(const solve_field subproblem,solve_field polyomino){
    solve_field reachable_matrix = polyomino.GetReachable();
    reachable_matrix = ( reachable_matrix^subproblem ) & reachable_matrix;
    std::vector< Point > reachable;
    for(int i=0;i<PETTERN_MATCH_MAX_HEIGHT;i++){
        for(int j=0;j<PETTERN_MATCH_MAX_WIDTH;j++){
            if(reachable_matrix[i][j]==1)reachable.push_back(Point(j,i));
        }
    }
    
    return reachable;
}

CONSTEXPR_RIYA std::vector< Point > PetternSolver::getReachable(const solve_field subproblem,solve_field polyomino,Point target){
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

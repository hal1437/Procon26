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
#include"../Structure/module.hpp"
#include"../Utility/CLOCKWISE_FOR.hpp"

#define PETTERN_MATCH_MAX_WIDTH 3
#define PETTERN_MATCH_MAX_HEIGHT 3

#define CONSTEXPR_RIYA


constexpr int hash_size = Anct::pow(2,PETTERN_MATCH_MAX_WIDTH*PETTERN_MATCH_MAX_HEIGHT);

using block_hash = size_t;
//using block_list = std::array< block_hash , 512 >;
using solve_field = Matrix< PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>;
using petternTable = Anct::Array< Anct::Array< block_hash ,hash_size>, hash_size>;


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
    
private:
    static CONSTEXPR_RIYA petternTable solvePettern();
    
public:
    
    CONSTEXPR_RIYA PetternSolver():_Table(solvePettern()){}
    petternTable _Table;
    
    //static CONSTEXPR_RIYA int MAX_WIDTH=3;
    //static CONSTEXPR_RIYA int MAH_HEIGHT=3;
    
    bool isMatched(petternTable& table,size_t subproblem_hash,std::vector<solve_field> block_list);
    
private:
    std::vector<solve_field> removeElement(std::vector<solve_field> list,int i);
    static CONSTEXPR_RIYA void solveSubproblem(solve_field subproblem, petternTable& table);
    static CONSTEXPR_RIYA Anct::Array< Point,4 > getReachable(const solve_field subproblem,solve_field polyomino);
    static CONSTEXPR_RIYA Anct::Array< Point,4 > getReachable(const solve_field subproblem,solve_field polyomino,Point target);
    static CONSTEXPR_RIYA solve_field addPointToMatrix(solve_field source,Point pos);
    static CONSTEXPR_RIYA Anct::Array< solve_field,4 > getPolyominoList(solve_field& polyomino,Anct::Array<Point,4> reachable);
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
        auto elem = table[subproblem_hash][Anct::hash<PETTERN_MATCH_MAX_WIDTH,PETTERN_MATCH_MAX_HEIGHT>()(block_list[i])];
        if(elem != 0){
            std::cout << block_list[i] << std::endl;
            std::cout << solve_field(elem) << std::endl;
            if(isMatched(table,elem,removeElement(block_list, i)))return true;
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
    Anct::Array < Point,4 > reachable;
    Anct::Array< solve_field,hash_size> polyomino_queue;
    solve_field polyomino;
    size_t problem_hash = Anct::hash<PETTERN_MATCH_MAX_WIDTH, PETTERN_MATCH_MAX_HEIGHT>()(subproblem);
    auto source = subproblem;
    
    int itr=1;
    
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
                polyomino_queue[0] = addPointToMatrix(solve_field(),Point(j,i));
                i=PETTERN_MATCH_MAX_HEIGHT; j=PETTERN_MATCH_MAX_WIDTH; //breakloop
            }
        }
    }
    
    if(polyomino_queue[0].count() == 0)return;
    
    while(polyomino_queue[0].count() != 0){
        polyomino = polyomino_queue[0];
        polyomino_queue = Anct::arrayTruncate<solve_field,hash_size>(polyomino_queue,1);
        itr--;
        //subproblem[target.y][target.x]=1;
        
        /*
        std::cout << "-- solve problem --" << std::endl;
        std::cout << source << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << polyomino << std::endl;
        std::cout << subproblem << std::endl;
        std::cout << "-----" << std::endl;
         */
        
        solveSubproblem( subproblem | polyomino , table );
        table[problem_hash][Anct::hash<PETTERN_MATCH_MAX_WIDTH,PETTERN_MATCH_MAX_HEIGHT>()(polyomino)] = Anct::hash<PETTERN_MATCH_MAX_WIDTH,PETTERN_MATCH_MAX_HEIGHT>()(subproblem | polyomino);
        
        reachable = getReachable(source, polyomino);
        auto poly_list = getPolyominoList(polyomino, reachable);
        for(auto _polyomino : poly_list){
            if(_polyomino.count() == 0)break;
            itr++;
            polyomino_queue[itr]= _polyomino;
        }
    }
}

CONSTEXPR_RIYA solve_field PetternSolver::addPointToMatrix(solve_field source,Point pos){
    source[pos.y][pos.x] = 1;
    return source;
}

CONSTEXPR_RIYA Anct::Array< solve_field,4 > PetternSolver::getPolyominoList(solve_field& polyomino,Anct::Array<Point,4> reachable){
    Anct::Array< solve_field,4 > list;
    int count=0;
    
    for(int i=0;i<4;i++){
        if(reachable[i].x >= 0){
            list[count] =  addPointToMatrix(polyomino, reachable[i]);
            count++;
        }
    }
    
    return list;
}

CONSTEXPR_RIYA Anct::Array< Point,4 > PetternSolver::getReachable(const solve_field subproblem,solve_field polyomino){
    solve_field reachable_matrix = polyomino.GetReachable();
    reachable_matrix = ( reachable_matrix^subproblem ) & reachable_matrix;
    Anct::Array< Point,4 > reachable;
    int count=0;
    for(int i=0;i<PETTERN_MATCH_MAX_HEIGHT;i++){
        for(int j=0;j<PETTERN_MATCH_MAX_WIDTH;j++){
            if(reachable_matrix[i][j]==1){
                reachable[count] = Point(j,i);
                count++;
            }
        }
    }
    
    return reachable;
}

CONSTEXPR_RIYA Anct::Array< Point,4 > PetternSolver::getReachable(const solve_field subproblem,solve_field polyomino,Point target){
    Anct::Array< Point,4 > reachable;
    int count = 0;
    
    CLOCKWISE_FOR(direction){
        Point pos = target + direction;
        if(pos.x != PETTERN_MATCH_MAX_WIDTH-1 && pos.x != 0 && pos.y != PETTERN_MATCH_MAX_HEIGHT-1 && pos.y != 0){
            if(subproblem[pos.y][pos.x] == 0 && polyomino[pos.y][pos.x] == 0){
                reachable[count] = pos;
            }
        }
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

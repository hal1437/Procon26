//
//  isContain.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/06.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_project_isContain_h
#define Procon26_project_isContain_h

#include"../Structure/Problem.h"
#include"../Structure/Matrix.hpp"
#include"../Structure/Transform.h"

void printBlock(const std::vector< std::vector<int> >& block);
std::vector< std::vector<int> > xorField(const std::vector< std::vector<int> >& rhs, const std::vector< std::vector<int> >& lhs);
std::vector< std::vector<int> > particalBlock(const std::vector< std::vector<int> >& block);
template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > particalBlock(const Matrix<WIDTH,HEIGHT>& block);
std::vector< std::vector<int> > particalBlock(const std::vector< std::vector<int> >& block);
template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > particalClosedField(const Matrix<WIDTH,HEIGHT>& block);
template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > particalClosedField(const Matrix<WIDTH,HEIGHT>& block);
std::vector< std::vector<int> > particalClosedField(const std::vector< std::vector<int> >& block);
std::vector< std::vector<int> > convertBlock(const std::vector< std::vector<int> >& field);

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > particalBlock(const Matrix<WIDTH,HEIGHT>& block){
    std::vector< std::vector<int> > parted_block;
    bool isTrimed=true;
    
    if(block.count()==0)return parted_block;
    
    parted_block.resize(HEIGHT);
    for(int i=0; i<HEIGHT; i++){
        parted_block[i].resize(WIDTH);
        for(int j=0; j<WIDTH; j++){
            parted_block[i][j] = block[i][j];
        }
    }
    
    while(isTrimed){
        isTrimed=false;
        for(int rows: {int(parted_block.size()-1),0} ){
            if(parted_block.size()==0)break;
            for(int j=0; j<parted_block[rows].size(); j++){
                if(parted_block[rows][j] == 1){
                    break;
                }
                if(j==parted_block[rows].size()-1){
                    parted_block.erase(parted_block.begin()+rows);
                    isTrimed = true;
                    break;
                }
            }
        }
        for(int cols: {int(parted_block[0].size()-1),0} ){
            if(parted_block[0].size()==0)break;
            for(int i=0; i<parted_block.size(); i++){
                if(parted_block[i][cols] == 1){
                    break;
                }
                if(i==parted_block.size()-1){
                    for(int k=0; k<parted_block.size(); k++){
                        parted_block[k].erase(parted_block[k].begin()+cols);
                        isTrimed = true;
                    }
                }
            }
        }
    }
    
    return parted_block;
}

template<std::size_t WIDTH,std::size_t HEIGHT>
std::vector< std::vector<int> > particalClosedField(const Matrix<WIDTH,HEIGHT>& block){
    std::vector< std::vector<int> > parted_block;
    parted_block.resize(HEIGHT);
    bool isTrimed=true;
    
    if(block.count()==0)return parted_block;
    
    for(int i=0; i<HEIGHT; i++){
        parted_block[i].resize(WIDTH);
        for(int j=0; j<WIDTH; j++){
            parted_block[i][j] = block[i][j];
        }
    }
    
    for(int i=0; i<HEIGHT; i++){
        parted_block[i].resize(WIDTH);
        for(int j=0; j<WIDTH; j++){
            parted_block[i][j] = block[i][j];
        }
    }
    
    while(isTrimed){
        isTrimed=false;
        for(int rows: {int(parted_block.size()-1),0} ){
            if(parted_block.size()==0)break;
            for(int j=0; j<parted_block[rows].size(); j++){
                if(parted_block[rows][j] == 0){
                    break;
                }
                if(j==parted_block[rows].size()-1){
                    parted_block.erase(parted_block.begin()+rows);
                    isTrimed = true;
                    break;
                }
            }
        }
        for(int cols: {int(parted_block[0].size()-1),0} ){
            if(parted_block[0].size()==0)break;
            for(int i=0; i<parted_block.size(); i++){
                if(parted_block[i][cols] == 0){
                    break;
                }
                if(i==parted_block.size()-1){
                    for(int k=0; k<parted_block.size(); k++){
                        parted_block[k].erase(parted_block[k].begin()+cols);
                        isTrimed = true;
                    }
                }
            }
        }
    }
    return parted_block;
}


#endif
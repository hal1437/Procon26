//
//  blockUtility.cpp
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/08.
//  Copyright © 2015年 Riya.Liel. All rights reserved.
//

#include"blockUtility.h"

void printBlock(const std::vector< std::vector<int> >& block){
    std::cout << std::endl;
    for(int i=0; i<block.size(); i++){
        for(int j=0; j<block[i].size(); j++){
            std::cout << block[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector< std::vector<int> > xorField(const std::vector< std::vector<int> >& rhs, const std::vector< std::vector<int> >& lhs){
    std::vector< std::vector<int> > result;
    
    result.resize(rhs.size());
    for(int i=0; i<result.size(); i++){
        result[i].resize(rhs[i].size());
        for(int j=0; j<result[i].size(); j++){
            result[i][j] = rhs[i][j] ^ lhs[i][j];
        }
    }
    return result;
}


std::vector< std::vector<int> > particalBlock(const std::vector< std::vector<int> >& block){
    std::vector< std::vector<int> > parted_block(block);
    
    if(block.size()==0)return parted_block;
    bool isTrimed=true;
    
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


std::vector< std::vector<int> > particalClosedField(const std::vector< std::vector<int> >& block){
    std::vector< std::vector<int> > parted_block(block);

    bool isTrimed=true;
    
    if(block.size()==0)return parted_block;
    
    while(isTrimed){
        isTrimed=false;
        for(std::size_t rows: {parted_block.size() - 1,0UL} ){
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
        for(std::size_t cols: {int(parted_block[0].size()-1),0} ){
            if(parted_block[0].size()==0)break;
            for(int i=0; i<parted_block.size(); i++){
                if(parted_block[i][cols] == 0){
                    break;
                }
                if(i==parted_block.size()-1){
                    for(int k=0; k<parted_block.size(); k++){
                        parted_block[k].erase(parted_block[k].begin());
                        isTrimed = true;
                    }
                }
            }
        }
    }
    
    return parted_block;
}

std::vector< std::vector<int> > convertBlock(const std::vector< std::vector<int> >& field){
    std::vector< std::vector<int> > block;
    block.resize(field.size());
    for(int i=0; i<field.size(); i++){
        block[i].resize(field[i].size());
        for(int j=0; j<field[i].size(); j++){
            block[i][j] = field[i][j] ? 0 : 1;
        }
    }
    return block;
}


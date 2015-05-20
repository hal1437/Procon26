#include "tsp.h"
#include <random>

tsp_individual* tsp_individual::cross_over(GA_Base* source){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<int> distribution(0,static_cast<int>(_phenotypic_trait.size()-1));
    
    int cross_point = distribution(mt);
    
    tsp_individual* child = new tsp_individual;
    child->_phenotypic_trait = this->_phenotypic_trait;
    
    for(int i=cross_point;i<_phenotypic_trait.size();i++) {
        child->_phenotypic_trait[i] = dynamic_cast<tsp_individual*>(source)->_phenotypic_trait[i];
    }
    
    return child;
}

void tsp_individual::mutation(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<int> distribution(0,static_cast<int>(_phenotypic_trait.size()-1));

    int mutation_key[2];
    
    DNA phenotypic_ordinal = translateToDnaPhenotypicOrdinal(_phenotypic_trait);
    
    do{
        mutation_key[0] = distribution(mt);
        mutation_key[1] = distribution(mt);
    }while(mutation_key[0] != mutation_key[1]);
    
    std::swap(phenotypic_ordinal[mutation_key[0]], phenotypic_ordinal[mutation_key[1]]);
    
    _phenotypic_trait = translateToDnaPhenotypicTrait(phenotypic_ordinal);
}

void tsp_individual::inversion(){
    DNA phenotypic_ordinal = translateToDnaPhenotypicOrdinal(_phenotypic_trait);

    std::swap(*phenotypic_ordinal.begin(), *(phenotypic_ordinal.end()-1));
    
    _phenotypic_trait = translateToDnaPhenotypicTrait(phenotypic_ordinal);
}

tsp_individual::DNA tsp_individual::translateToDnaPhenotypicOrdinal(DNA trait){
    DNA ordinal;
    DNA tmp;
    
    for(int i=0;i<trait.size();i++){
        tmp.push_back(i);
    }
    
    ordinal.resize(trait.size());
    
    for(int i=0;i<trait.size();i++){
        ordinal[i] = tmp[trait[i]];
        tmp.erase(tmp.begin() + trait[i]);
    }
    
    return ordinal;
}

tsp_individual::DNA tsp_individual::translateToDnaPhenotypicTrait(DNA ordinal){
    DNA trait;
    DNA tmp;
    
    for(int i=0;i<trait.size();i++){
        tmp.push_back(i);
    }
    
    trait.resize(ordinal.size());
    
    for(int i=0;i<trait.size();i++){
        for(int j=0;j<trait.size();j++){
            if(trait[i] == tmp[j]){
                ordinal.push_back(j);
                tmp.erase(tmp.begin() + j);
                break;
            }
        }
    }

    
    return trait;
}

tsp_individual* makeTspIndividual(int number_of_city){
    std::vector<int> city_list;
    tsp_individual::DNA new_dna;
    tsp_individual* new_indivisual = new tsp_individual;
    int pos;
    
    for(int i=0;i<number_of_city;i++){
        city_list.push_back(i);
    }
    
    for(int i=0;i<number_of_city;i++){
        pos = rand() % city_list.size();
        new_dna.push_back(pos);
        city_list.erase(city_list.begin() + pos);
    }
    
    new_indivisual->setDNA(new_dna);
    
    return new_indivisual;
}

int tsp_individual::calcEvalution(void* aux){
    int evalution=0;
    std::vector<cv::Point>* city_list = reinterpret_cast<std::vector<cv::Point>*>(aux);
    if(!city_list)throw aux;
    
    DNA phenotypic_ordinal = translateToDnaPhenotypicOrdinal(_phenotypic_trait);
    
    for(int i=0;i<city_list->size()-1;i++){
        evalution+= cv::norm((*city_list)[phenotypic_ordinal[i]]-(*city_list)[phenotypic_ordinal[i+1]]);
    }
    
    evalution+= cv::norm((*city_list)[phenotypic_ordinal.back()]-(*city_list)[phenotypic_ordinal[0]]);

    return evalution;
}
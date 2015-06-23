<<<<<<< HEAD
#include <iostream>
#include <random>
#include "Riya/tsp_ga.h"
#include "Riya/tsp_sa.h"
#include "Riya/block_sa_solver.h"
#include "Riya/GA/GASolver.hpp"
#include "Riya/SA/SASolver.hpp"
#include "Riya/petternMatching.h"
#include "Structure/Problem.h"
#include "Structure/Point.h"
#include "Structure/Matrix.hpp"

//#define SOLVE_BY_GA
//#define SOLVE_BY_SA
#define Procon

#ifdef SOLVE_BY_GA

int main(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distiribution(0,500);
    std::vector<cv::Point> city_list;
    std::vector<tsp_individual*> population;
    int count=1;
    const int max_age=500,city_size=80,individual_size=city_size;
    
     for(int i=0;i<city_size;i++){
     city_list.push_back(cv::Point(distiribution(mt),distiribution(mt)));
     }
    
    /*
    for(int i=0; i<city_size; i++){
        city_list.push_back(cv::Point(250+200*cos(i*(2*3.14)/city_size),250+200*sin(i*(2*3.14)/city_size)));
    }
     */
    
    for(int i=0;i<individual_size;i++){
        population.push_back(makeTspIndividual(city_size));
    }
    
    std::cout << cv::norm(city_list[0]-city_list[1]) << std::endl;
    
    GA_Solver<tsp_individual,individual_size> solver(population);
    solver.setAux(city_list);
    
    solver.populationSettings();
    tsp_individual::DNA root = solver.getPopulation().front()->getPhenotypic();
    while(true){
        std::cout << count * max_age << "世代目" << std::endl;
        
        cv::Mat drawImage(500, 500, CV_8UC3, cv::Scalar(0));
        for(cv::Point p : city_list)
        {
            cv::line(drawImage, p, p, cv::Scalar(255, 0, 0),5);
        }
        
        for(int i=0;i<root.size();i++)
        {
            std::cout << root[i] << ' ';
        }
        std::cout << std::endl;
        
        int eval=0;
        for(int i=0;i<root.size()-1;i++)
        {
            cv::line(drawImage, city_list[root[i]], city_list[root[i+1]], cv::Scalar(255, 0, 0),1);
            eval += cv::norm(city_list[root[i]]-city_list[root[i+1]]);
        }
        eval += cv::norm(city_list[root.front()]-city_list[root.back()]);
        std::cout << eval << std::endl;
        
        cv::line(drawImage, city_list[root.back()], city_list[root.front()], cv::Scalar(255, 0, 0),1);
        cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
        cv::imshow("test", drawImage);
        while (true)
        {
            int key = cv::waitKey(1);
            if (key == 0x1b) break;
        }
        count++;
        
        tsp_individual* answer = dynamic_cast<tsp_individual*>(solver.solveAnswer(max_age));
        root =  answer->getPhenotypic();
    }
    
    return 0;
}

#endif

#ifdef SOLVE_BY_SA

int main(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<int> distiribution(0,500);
    std::vector<cv::Point> city_list;
    std::vector<int> city_hash,ans;
    int count=1,pos;
    const int max_age=500,city_size=50;
    
    
    for(int i=0;i<city_size;i++){
        city_list.push_back(cv::Point(distiribution(mt),distiribution(mt)));
    }
    
    for(int i=0;i<city_size;i++){
        city_hash.push_back(i);
    }
    
    for(int i=0;i<city_size;i++){
        pos = mt() % city_hash.size();
        ans.push_back(city_hash[pos]);
        city_hash.erase(city_hash.begin() + pos);
    }
    
    tsp_annealing target(ans);
    
    SA_Solver<tsp_annealing> solver( target );
    solver.setAux(city_list);
    
    ans = solver.solveAnswer();
        
        cv::Mat drawImage(500, 500, CV_8UC3, cv::Scalar(0));
        for(cv::Point p : city_list)
        {
            cv::line(drawImage, p, p, cv::Scalar(255, 0, 0),5);
        }
        
        for(int i=0;i<ans.size();i++)
        {
            std::cout << ans[i] << ' ';
        }
        std::cout << std::endl;
        
        int eval=0;
        for(int i=0;i<ans.size()-1;i++)
        {
            cv::line(drawImage, city_list[ans[i]], city_list[ans[i+1]], cv::Scalar(255, 0, 0),1);
            eval += cv::norm(city_list[ans[i]]-city_list[ans[i+1]]);
        }
        eval += cv::norm(city_list[ans.front()]-city_list[ans.back()]);
        std::cout << eval << std::endl;
        
        cv::line(drawImage, city_list[ans.back()], city_list[ans.front()], cv::Scalar(255, 0, 0),1);
        cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
        cv::imshow("test", drawImage);
        while (true)
        {
            int key = cv::waitKey(1);
            if (key == 0x1b) break;
        }
    return 0;
}

#endif

#ifdef Procon

int main(){
    /*
    std::map <int ,std::map<int , int> > a;
    std::vector< std::vector<int> > problem;
    
    problem.resize(2);
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            problem[i].resize(2);
            problem[i][j] = 0;
        }
    }*/
    
    //petternTable table = solvePettern(problem);
    
    Problem prob("Problem/quest7.txt");
    
    Block_SA target( (Answer_history<Transform, Block>()) );
    
    SA_Solver<Block_SA,100,1,90> solver( target );
    solver.setAux(prob);
     
    target = solver.solveAnswer();
    
    auto ans = target.getState().TranslateAnswer(prob);
    
    std::cout << (target.getField() | prob.GetField()) << std::endl;
    
    ans.Export("Answer.txt");
    
    std::cout << std::endl;
}

#endif
=======

#include <iostream>
#include <fstream>
#include <random>
#include "Structure/Problem.h"
#include "Structure/Answer.h"
#include "Structure/Solver.h"
#include "Structure/Matrix.hpp"
#include "Utility/BenchMark.hpp"

int main(){

	Problem prob("Problem/quest9.txt");
	std::ofstream ofs("Answer.txt");
	Field field;// = prob.GetField();
	Answer ans(prob);
	
	//===========BENCHMARK RESULT===========
	//    [COUNT]          500 times
	//[FULL TIME]         9756 msec
	// [PER TIME]       19.512 msec/function 
	//======================================
	
	std::cout << prob.GetField() << std::endl;
	BenchMark<500>()([&](){
		prob.GetField().GetListLayPossible(prob.GetBlock(0));
	});
	//std::cout << prob.GetField() << std::endl;
	
	return 0;
}
>>>>>>> 1e691c36fa59b64c845413756bf61ba0d6867d10

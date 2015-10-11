//
//  TUSolver.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/06/18.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_project_TUSolver_h
#define Procon26_project_TUSolver_h

#include<vector>

template<class _Tp,class _ChangeState>
class TUSolver{
    const int NEIGHTBORHOOD,TABU_LIST_SIZE;
    using state_type = _Tp::_State;
    using aux_type = _Tp::_Aux;
    
    std::vector<_ChangeState> _tabu_list;
    
public:
    TUSolver(const int NEIGHTBORHOOD,LIST_SIZE): NEIGHTBORHOOD(NEIGHTBORHOOD),LIST_SIZE(LIST_SIZE){}
    TUSolver(): NEIGHTBORHOOD(5),LIST_SIZE(100){}
    state_type solveAnswer();
};
#endif

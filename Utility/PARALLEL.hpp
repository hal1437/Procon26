//
//  PARALLEL.h
//  Procon26_project
//
//  Created by Riya.Liel on 2015/10/08.
//  Copyright © 2015年 Riya.Liel. All rights reserved.
//

#ifndef PARALLEL_h
#define PARALLEL_h

#define PARALLEL_FOR(_BEGIN_INDEX,_END_INDEX,_FUNCTION)                     \
int _m = std::max(std::thread::hardware_concurrency(),1u);                  \
std::vector<std::thread> worker;                                            \
int _N = _END_INDEX - _BEGIN_INDEX;                                         \
for(int _i=0; _i<_m; _i++){                                                 \
    worker.emplace_back([&](int core_id){                                   \
        int _r0 = _N/_m * core_id     + std::min(_N%_m,core_id);            \
        int _r1 = _N/_m * (core_id+1) + std::min(_N%_m,core_id+1);          \
        for(int P_IT=_r0+_BEGIN_INDEX; P_IT<_r1+_BEGIN_INDEX; P_IT++){      \
            _FUNCTION                                                       \
        }                                                                   \
    },_i);                                                                  \
}                                                                           \
for(auto& t: worker)t.join();

#endif /* PARALLEL_h */

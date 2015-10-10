//
//  module.hpp
//  Procon26_project
//
//  Created by Riya.Liel on 2015/06/30.
//  Copyright (c) 2015年 Riya.Liel. All rights reserved.
//

#ifndef Procon26_project_module_hpp
#define Procon26_project_module_hpp

namespace Anct{
    
    template<class T>
    constexpr T pow(T base, T exp) noexcept {
        //static_assert(exp >= 0, "Exponent must not be negative");
        return exp <= 0 ? 1
        :  exp == 1 ? base
        :  base * pow(base, exp-1);
    }

    
    template<class T,int Size>
        constexpr Anct::Array<T,Size> arrayTruncate(Anct::Array<T,Size> source,int n){
            for(int i=n;i<Size;i++){
                source[i-n] = source[i];
            }
            return source;
        }
    
}
#endif

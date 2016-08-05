//
//  error.cpp
//  LLVMParser
//
//  Created by Mingbao on 8/5/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#include "error.hpp"
#include "scanner.hpp"

namespace llvmpascal
{
    void errorToken(const std::string& msg)
    {
        std::cerr<<"Token Error:"<<msg<<std::endl;
        Scanner::setErrorFlag(true);
    }
    
    void errorSyntax(const std::string& msg)
    {
        std::cerr<<"Syntax Error:"<<msg<<std::endl;
    }
}
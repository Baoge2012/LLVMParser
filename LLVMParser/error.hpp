//
//  error.hpp
//  LLVMParser
//
//  Created by Mingbao on 8/5/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#ifndef error_hpp
#define error_hpp

#include <stdio.h>
#include <string>
#include <cassert>
#include <iostream>

namespace llvmpascal
{
    extern void errorToken(const std::string& msg);
    extern void errorSyntax(const std::string& msg);
}

#endif /* error_hpp */

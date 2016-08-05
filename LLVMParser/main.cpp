//
//  main.cpp
//  LLVMParser
//
//  Created by Mingbao on 8/4/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#include <iostream>
#include "scanner.hpp"
using namespace llvmpascal;

int main(int argc, const char * argv[]) {
    // insert code here...
    Scanner scanner("/Users/mingbao/JustForFun/LLVMParser/LLVMParser/Scanner_test.txt");
    while (scanner.getToken().getTokenType() != TokenType::END_OF_FILE)
    {
        scanner.getToken().dump();
        scanner.getNextToken();
    }
    return 0;
}

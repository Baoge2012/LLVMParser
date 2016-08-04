//
//  Dictionary.hpp
//  LLVMParser
//
//  Created by Mingbao on 8/4/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#ifndef Dictionary_hpp
#define Dictionary_hpp

#include <stdio.h>
#include <string>
#include <tuple>
#include <map>
#include "Token.hpp"

namespace llvmpascal
{
    class Dictionary
    {
    public:
        Dictionary();
        std::tuple<TokenType, TokenValue, int> lookup(const std::string& name) const;
        bool haveToken(const std::string& name) const;
        
    private:
        void addToken(std::string name, std::tuple<TokenValue, TokenType, int> tokenMeta);
        
    private:
        std::map<std::string, std::tuple<TokenValue, TokenType, int>> dictionary_;
    };
    
}

#endif /* Dictionary_hpp */

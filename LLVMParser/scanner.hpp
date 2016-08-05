//
//  scanner.hpp
//  LLVMParser
//
//  Created by Mingbao on 8/5/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#ifndef scanner_hpp
#define scanner_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include "Token.hpp"
#include "Dictionary.hpp"

namespace llvmpascal
{
    class Scanner
    {
    public:
        explicit    Scanner(const std::string& srcFileName);
        Token       getToken() const;
        Token       getNextToken();
        static bool getErrorFlag();
        static void setErrorFlag(bool flag);
        
    private:
        void        getNextChar();
        char        peekChar();
        void        addToBuffer(char c);
        void        reduceBuffer();
        
        void        makeToken(TokenType tt, TokenValue tv, const TokenLocation& loc, std::string name, int symbolPrecedence);
        void        makeToken(TokenType tt, TokenValue tv,
                              const TokenLocation& loc, long intValue, std::string name);
        
        void        makeToken(TokenType tt, TokenValue tv,
                              const TokenLocation& loc, double realValue, std::string name);
        void        makeToken(TokenType tt, TokenValue tv,
                              const TokenLocation& loc, const std::string& strVale, std::string name);
        
        void        handleEOFState();
        void        handleIdentifierState();
        void        handleNumberState();
        void        handleStringState();
        void        handleOperationState();
        void        preprocess();
        void        handleLineComment();
        void        handleBlockComment();
        TokenLocation   getTokenLocation() const;
        
        void        handleDigit();
        void        handleXDigit();
        void        handleFraction();
        void        handleExponent();
        void        errorReport(const std::string& msg);
        
    public:
        enum class State
        {
            NONE,
            END_OF_FILE,
            IDENTIFIER,
            NUMBER,
            STRING,
            OPERATION
        };
        
    private:
        std::string     fileName_;
        std::ifstream   input_;
        long            line_;
        long            column_;
        TokenLocation   loc_;
        char            currentChar_;
        State           state_;
        Token           token_;
        Dictionary      dictionary_;
        std::string     buffer_;
        static bool     errorFlag_;
    };
    
    inline Token Scanner::getToken() const
    {
        return token_;
    }
    
    inline bool Scanner::getErrorFlag()
    {
        return errorFlag_;
    }
    
    inline TokenLocation Scanner::getTokenLocation() const
    {
        return TokenLocation(fileName_, (int)line_, (int)column_);
    }
}

#endif /* scanner_hpp */

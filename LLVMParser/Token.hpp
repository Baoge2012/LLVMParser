//
//  Token.hpp
//  LLVMParser
//
//  Created by Mingbao on 8/4/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include <stdio.h>
#include <iostream>
#include <tuple>
#include <map>
#include <cassert>

namespace llvmpascal
{
    enum class TokenType//词法单元类型
    {
        INTEGER,
        REAL,
        BOOLEAN,
        CHAR,
        STRING_LITERAL,
        
        IDENTIFIER,
        KEYWORDS,
        OPERATORS,
        DELIMITER,
        END_OF_FILE,
        UNKNOWN
    };
    
    enum class TokenValue//词法单元值
    {
        AND,
        FOR,
        TO,
        DOWNTO,
        DO,
        IF,
        THEN,
        ELSE,
        WHILE,
        REPEAT,
        UNTIL,
        FUNCTION,
        PROCEDURE,
        BEGIN,
        END,
        PROGRAM,
        FORWARD,
        GOTO,
        OR,
        NOT,
        CASE,
        OTHERWISE,
        WITH,
        IN,
        
        WRITE,
        WRITELN,
        READ,
        READLN,
        
        TYPE,
        VAR,
        PACKED,
        ARRAY,
        OF,
        RECORD,
        CONST,
        FILE,
        SET,
        STRING,
        NIL,
        
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_SQUARE,
        RIGHT_SQUARE,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        COMMA,
        SEMICOLON,
        COLON,
        ASSIGN,
        PERIOD,
        DOT_DOT,
        UPARROW,
        DIV,
        MOD,
        XOR,
        SHR,
        SHL,
        
        LESS_OR_EQUAL,
        LESS_THAN,
        GREATER_OR_EQUAL,
        GREATER_THAN,
        EQUAL,
        NOT_EQUAL,
        
        UNRESERVED
    };
    
    class TokenLocation
    {
    public:
        TokenLocation();
        TokenLocation(const std::string& fileName, int line, int column);
        
        std::string toString() const;
        
    private:
        std::string fileName_;
        int line_;
        int column_;
    };
    
    class Token
    {
    public:
        Token();
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              std::string name, int symbolPrecedence);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              const std::string& strValue, std::string name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              long intValue, std::string name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              double realValue, std::string name);

        TokenType getTokenType() const;
        TokenValue getTokenValue() const;
        const TokenLocation& getTokenLocation() const;
        std::string getTokenName() const;
        
        int getSymbolPrecedence() const;
        
        long getIntValue() const;
        double getRealValue() const;
        std::string getStringValue() const;
        
        void dump(std::ostream& out = std::cout) const;
        
        std::string getIdentifierName() const;
        
        std::string tokenTypeDescription() const;
        
        std::string toString() const;
        
    private:
        TokenType type_;
        TokenValue value_;
        TokenLocation location_;
        std::string name_;//词法单元名称
        int symbolPrecedence_;//词法单元符号优先级
        
        long intValue_;
        double realValue_;
        std::string strValue_;
    };
    
    inline TokenType Token::getTokenType() const
    {
        return type_;
    }
    
    inline TokenValue Token::getTokenValue() const
    {
        return value_;
    }
    
    inline std::string Token::getTokenName() const
    {
        return name_;
    }
    
    inline const TokenLocation& Token::getTokenLocation() const
    {
        return location_;
    }
    
    inline long Token::getIntValue() const
    {
        return intValue_;
    }
    
    inline double Token::getRealValue() const
    {
        return realValue_;
    }
    
    inline std::string Token::getStringValue() const
    {
        return strValue_;
    }
    
    inline int Token::getSymbolPrecedence() const
    {
        return symbolPrecedence_;
    }
    
    inline std::string Token::getIdentifierName() const
    {
        return name_;
    }
}

#endif /* Token_hpp */

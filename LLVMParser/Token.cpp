//
//  Token.cpp
//  LLVMParser
//
//  Created by Mingbao on 8/4/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#include "Token.hpp"

namespace llvmpascal
{
    TokenLocation::TokenLocation(const std::string& fileName, int line, int column)
    : fileName_(fileName), line_(line), column_(column)
    {}
    
    TokenLocation::TokenLocation() : fileName_(""), line_(1), column_(0)
    {}
    
    std::string TokenLocation::toString() const
    {
        return fileName_ + ":" + std::to_string(line_) + ":" + std::to_string(column_) + ":";
    }
    

    Token::Token() : type_(TokenType::UNKNOWN), value_(TokenValue::UNRESERVED),
    location_(std::string(""), 0, 0), name_(""), symbolPrecedence_(-1)
    {}
    
    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 std::string name, int symbolPrecedence)
    : type_(type), value_(value), location_(location), name_(name),
    symbolPrecedence_(symbolPrecedence)
    {}
    
    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 const std::string& strValue, std::string name)
    : type_(type), value_(value), location_(location),
    name_(name), symbolPrecedence_(-1), strValue_(strValue)
    {}
    
    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 long intValue, std::string name)
    : type_(type), value_(value), location_(location),
    name_(name), symbolPrecedence_(-1), intValue_(intValue)
    {}
    
    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 double realValue, std::string name)
    : type_(type), value_(value), location_(location),
    name_(name), symbolPrecedence_(-1), realValue_(realValue)
    {}
    
    std::string Token::tokenTypeDescription() const
    {
        std::string buffer;
        switch (type_) {
            case TokenType::INTEGER:
                buffer = "integer";
                break;
            case TokenType::REAL:
                buffer = "real";
                break;
            case TokenType::BOOLEAN:
                buffer = "boolean";
                break;
            case TokenType::CHAR:
                buffer = "char";
                break;
            case TokenType::STRING_LITERAL:
                buffer = "string literal";
                break;
            case TokenType::IDENTIFIER:
                buffer = "identifier";
                break;
            case TokenType::KEYWORDS:
                buffer = "keywords";
                break;
            case TokenType::OPERATORS:
                buffer = "operators";
                break;
            case TokenType::DELIMITER:
                buffer = "delemiter";
                break;
            case TokenType::END_OF_FILE:
                buffer = "eof";
                break;
            case TokenType::UNKNOWN:
                buffer = "unknow";
                break;
            default:
                break;
        }
        return buffer;
    }
    
    std::string Token::toString() const
    {
        return std::string("Token Type: " + tokenTypeDescription() + "Token Name: " + name_);
    }
    
    void Token::dump(std::ostream& out) const
    {
        out<<location_.toString()<<"\t"<<toString()<<"\t"<<tokenTypeDescription()<<"\t\t"<<getSymbolPrecedence()<<std::endl;
    }
}
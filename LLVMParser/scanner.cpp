//
//  scanner.cpp
//  LLVMParser
//
//  Created by Mingbao on 8/5/16.
//  Copyright © 2016 qiyi. All rights reserved.
//

#include "scanner.hpp"
#include "error.hpp"

namespace llvmpascal
{
    bool Scanner::errorFlag_ = false;
    
    Scanner::Scanner(const std::string& srcFileName):
    fileName_(srcFileName), line_(1), column_(0), currentChar_(0), state_(State::NONE)
    {
        input_.open(fileName_);
        if (input_.fail())
        {
            errorReport("When trying to open file "+ fileName_ + ", occourred error.");
        }
    }
    
    void Scanner::getNextChar()
    {
        currentChar_ = input_.get();
        if (currentChar_ == '\n')
        {
            line_++;
            column_ = 0;
        }
        else
        {
            column_++;
        }
    }
    
    char Scanner::peekChar()
    {
        char c = input_.peek();
        return c;
    }
    
    void Scanner::addToBuffer(char c)
    {
        buffer_.push_back(c);
    }
    
    void Scanner::reduceBuffer()
    {
        buffer_.pop_back();
    }
    
    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, std::string name, int symbolPrecedence)
    {
        token_ = Token(tt, tv, loc, name, symbolPrecedence);
        buffer_.clear();
        state_ = State::NONE;
    }
    
    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, long intValue, std::string name)
    {
        token_ = Token(tt, tv, loc, intValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }
    
    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, double realValue, std::string name)
    {
        token_ = Token(tt, tv, loc, realValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }
    
    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, const std::string& strValue, std::string name)
    {
        token_ = Token(tt, tv, loc, strValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }

    void Scanner::preprocess()
    {
        do
        {
            while (std::isspace(currentChar_))
            {
                getNextChar();
            }
            handleLineComment();
            handleBlockComment();
        }while (std::isspace(currentChar_));
    }
    
    void Scanner::handleLineComment()
    {
        loc_ = getTokenLocation();
        if (currentChar_ == '(' && peekChar() == '*')
        {
            getNextChar();
            getNextChar();
            while (!(currentChar_ == '*' && peekChar() == ')'))
            {
                getNextChar();
                if (input_.eof())
                {
                    errorReport(std::string("end of file happended in comment, *) is expected!, but find ") + currentChar_);
                    break;
                }
            }
            
            if (!input_.eof())
            {
                getNextChar();
                getNextChar();
            }
        }
    }
    
    void Scanner::handleBlockComment()
    {
        loc_ = getTokenLocation();
        if (currentChar_ == '{')
        {
            do
            {
                getNextChar();
                if (input_.eof())
                {
                    errorReport(std::string("end of file happended in comment, } is expected!, but find ") + currentChar_);
                    break;
                }
            } while (currentChar_ != '}');
        }
    }
    
    Token Scanner::getNextToken()
    {
        bool matched = false;
        do
        {
            if (state_ != State::NONE)
            {
                matched = true;
            }
            switch (state_)
            {
                case State::NONE:
                    getNextChar();
                    break;
                case State::END_OF_FILE:
                    handleEOFState();
                    break;
                case State::IDENTIFIER:
                    handleIdentifierState();
                    break;
                case State::NUMBER:
                    handleNumberState();
                    break;
                case State::STRING:
                    handleStringState();
                    break;
                case State::OPERATION:
                    handleOperationState();
                    break;
                default:
                    errorReport("Match token state error.");
                    break;
            }
            if (state_ == State::NONE)
            {
                preprocess();
                if (input_.eof())
                {
                    state_ = State::END_OF_FILE;
                }
                else
                {
                    if (std::isalpha(currentChar_))
                    {
                        state_ = State::IDENTIFIER;
                    }
                    else if (std::isdigit(currentChar_) || (currentChar_ == '$'))
                    {
                        state_ = State::NUMBER;
                    }
                    else if (currentChar_ == '\'')
                    {
                        state_ = State::STRING;
                    }
                    else
                    {
                        state_ = State::OPERATION;
                    }
                }
            }
        } while (!matched);
        
        return token_;
    }
    
    void Scanner::handleEOFState()
    {
        loc_ = getTokenLocation();
        makeToken(TokenType::END_OF_FILE, TokenValue::UNRESERVED, loc_, std::string("END_OF_FILE"), -1);
        input_.close();
    }
    
    void Scanner::handleNumberState()
    {
        loc_ = getTokenLocation();
        bool isFloat = false;
        bool isExponent = false;
        int numberBase = 10;
        if (currentChar_ == '$')
        {
            numberBase = 16;
            getNextChar();
        }
        
        enum class NumberState
        {
            INTERGER,
            FRACTION,
            EXPONENT,
            DONE
        };
        
        NumberState numberState = NumberState::INTERGER;
        do {
            switch (numberState)
            {
                case NumberState::INTERGER:
                    if (numberBase == 10)
                    {
                        handleDigit();
                    }
                    else if (numberBase == 16)
                    {
                        handleXDigit();
                    }
                    break;
                case NumberState::FRACTION:
                    handleFraction();
                    isFloat = true;
                    break;
                case NumberState::EXPONENT:
                    handleExponent();
                    isExponent = true;
                    break;
                case NumberState::DONE:
                    
                    break;
 
                default:
                    errorReport("Match number state error.");
                    break;
            }
            if (currentChar_ == '.')
            {
                if (isFloat)
                {
                    errorReport("Fraction number can not have more than one dot.");
                }
                if (isExponent)
                {
                    errorReport("Scientist number representation in Pascal can not have dot.");
                }
                if (numberBase == 16)
                {
                    errorReport("Hexadecimal number in Pascal can only be integer.");
                }
                numberState = NumberState::FRACTION;
            }
            else if (currentChar_ == 'E' || currentChar_ == 'e')
            {
                if (isExponent)
                {
                    errorReport("Scientist presentation can not have more than one e / E");
                }
                numberState = NumberState::EXPONENT;
            }
            else
            {
                numberState = NumberState::DONE;
            }
        } while (numberState != NumberState::DONE);
        
        if (!getErrorFlag())
        {
            if (isFloat || isExponent)
            {
                makeToken(TokenType::REAL, TokenValue::UNRESERVED, loc_,
                          std::stod(buffer_), buffer_);
            }
            else
            {
                makeToken(TokenType::INTEGER, TokenValue::UNRESERVED, loc_,
                          std::stol(buffer_, 0, numberBase), buffer_);
            }
        }
        else
        {
            buffer_.clear();
            state_ = State::NONE;
        }
    }
    
    void Scanner::handleStringState()
    {
        loc_ = getTokenLocation();
        getNextChar();
        while (true)
        {
            if (currentChar_ == '\'')
            {
                if (peekChar() == '\'')
                {
                    getNextChar();
                }
                else
                {
                    break;
                }
            }
            addToBuffer(currentChar_);
            getNextChar();
        }
        
        getNextChar();
        
        if (buffer_.length() == 1)
        {
            makeToken(TokenType::CHAR, TokenValue::UNRESERVED, loc_,
                      static_cast<long>(buffer_.at(0)), buffer_);
        }
        else
        {
            makeToken(TokenType::STRING_LITERAL, TokenValue::UNRESERVED,
                      loc_, buffer_, buffer_);
        }
    }
    
    void Scanner::handleIdentifierState()
    {
        loc_ = getTokenLocation();
        addToBuffer(currentChar_);
        getNextChar();
        while (std::isalnum(currentChar_) || currentChar_=='_')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        
        std::transform(buffer_.begin(), buffer_.end(), buffer_.begin(), ::tolower);
        auto tokenMeta = dictionary_.lookup(buffer_);
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
    }
    
    void Scanner::handleOperationState()
    {
        loc_ = getTokenLocation();
        addToBuffer(currentChar_);
        addToBuffer(peekChar());

        if (dictionary_.haveToken(buffer_))
        {
            getNextChar();
        }
        else
        {
            reduceBuffer();
        }
        
        auto tokenMeta = dictionary_.lookup(buffer_);
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
        getNextChar();
    }
    
    void Scanner::handleDigit()
    {
        addToBuffer(currentChar_);
        getNextChar();
        
        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }
    
    void Scanner::handleXDigit()
    {
        bool readFlag = false;
        while (std::isxdigit(currentChar_))
        {
            readFlag = true;
            addToBuffer(currentChar_);
            getNextChar();
        }
        if (!readFlag)
        {
            errorReport("Hexadecimal number format error.");
        }
    }
    
    void Scanner::handleFraction()
    {
        if (!std::isdigit(peekChar()))
        {
            errorReport("Fraction number part should be numbers");
        }
        
        addToBuffer(currentChar_);
        getNextChar();
        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }
    
    void Scanner::handleExponent()
    {
        addToBuffer(currentChar_);
        getNextChar();
        
        if (currentChar_ != '+' && currentChar_ != '-' && !std::isdigit(currentChar_))
        {
            errorReport(std::string("Scientist presentation number after e / E should be + / - or digits but find ") + '\'' + currentChar_ + '\'');
        }
        
        if (currentChar_ == '+' || currentChar_ == '-')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        
        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }
    
    void Scanner::errorReport(const std::string& msg)
    {
        errorToken(getTokenLocation().toString() + msg);
    }
    
    void Scanner::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }
}
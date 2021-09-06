#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "../util/std_include.h"
#include "TokenType.h"

class Token {
   private:
    TokenType _type;
    string _value;

   public:
    Token(TokenType type, string value);
    ~Token();

    TokenType getType();
    string getValue();
    void display();
};

#endif
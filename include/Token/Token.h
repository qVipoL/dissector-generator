#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "../std_include.h"
#include "TokenType.h"

class Token {
   private:
    TokenType _type;
    string _value;

   public:
    Token(TokenType type, string value);
    ~Token();
    void display();
};

#endif
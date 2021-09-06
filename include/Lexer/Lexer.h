#ifndef __LEXER_H__
#define __LEXER_H__

#include "../Token/Token.h"
#include "../util/std_include.h"

class Lexer {
   private:
    string _content;
    int _content_length, _char_idx, _line_n;
    char _curr_char;

    void lexerAdvance();
    void skipWhitespace();
    void skipComment();

    Token *collectId();
    Token *collectNumber();
    Token *collectString();
    Token *advanceWithToken(TokenType type);

   public:
    Lexer(string content);
    ~Lexer();

    int getLineN();
    Token *getNextToken();
};

#endif
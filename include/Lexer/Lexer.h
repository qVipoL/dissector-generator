#ifndef __LEXER_H__
#define __LEXER_H__

#include "../Token/Token.h"
#include "../util/std_include.h"

class Lexer {
   private:
    string _content;
    int _content_length;
    int _char_idx;
    char _curr_char;
    int _line_n;

    void lexerAdvance();
    void skipWhitespace();
    void skipComment();
    Token collectNumber();
    Token collectId();
    Token collectString();
    Token advanceWithToken(TokenType type);

   public:
    Lexer(string content);
    ~Lexer();

    Token getNextToken();
};

#endif
#ifndef __PARSER_H__
#define __PARSER_H__

#include "../AST/AST.h"
#include "../Lexer/Lexer.h"
#include "../util/std_include.h"

/*
 * Parser
 * Usage - parses the tokens from the lexer into an abstract syntax tree
 * by the given grammar
 */
class Parser {
   private:
    Lexer *_lexer;
    Token *_curr_token;

    void eatToken(TokenType type);
    void eatToken(TokenType type, string value);

    AST *parseProto();
    AST *parseProtoDecl();

    AST *parseEndian();
    AST *parseProtoDetails();
    AST *parseDissectorEntry();
    AST *parseDissectorTable();

    AST *parseEnum();
    AST *parseEnumBody();
    AST *parseStruct();
    AST *parseStructBody();
    AST *parseLocalElement();
    AST *parseSwitch();
    AST *parseSwitchCondition();
    AST *parseFieldPath();
    AST *parseField();
    AST *parseCase();
    AST *parseCaseLabel();
    AST *parseCaseBody();

   public:
    Parser(Lexer *lexer);
    ~Parser();

    AST *parse();
};

#endif
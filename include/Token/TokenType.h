#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

enum TokenType {
    /* types */
    TOKEN_NUMBER,
    TOKEN_ID,
    TOKEN_STRING,

    /* operations */
    TOKEN_EQUALS_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_LESS_THAN,
    TOKEN_LARGER_THAN,

    /* separators */
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_EQUALS,
    TOKEN_COLON,
    TOKEN_SEMI_COLON,
    TOKEN_COMMA,

    TOKEN_EOF
};

#endif
#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

/* Token Types (defined in grammar.txt) */
enum TokenType {
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SEPARATOR,
    TOKEN_OPERATION,

    TOKEN_EOF
};

#endif
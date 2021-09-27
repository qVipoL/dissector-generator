#include "../include/Token/Token.h"

/* Public */

Token::Token(TokenType type, string value) {
    _type = type;
    _value = value;
}

Token::~Token() {
}

TokenType Token::getType() {
    return _type;
}

string Token::getValue() {
    return _value;
}

void Token::display() {
    cout << "(" << TOKEN_TYPES[_type] << ", " << _value << ")" << endl;
}

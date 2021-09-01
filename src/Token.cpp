#include "../include/Token/Token.h"

Token::Token(TokenType type, string value) {
    _type = type;
    _value = value;
}

Token::~Token() {
    // cout << "Token Destructed." << endl;
}

void Token::display() {
    cout << "(" << _type << ", " << _value << ")" << endl;
}

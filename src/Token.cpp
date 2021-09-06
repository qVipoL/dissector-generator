#include "../include/Token/Token.h"

/* Public */

Token::Token(TokenType type, string value) {
    _type = type;
    _value = value;
}

Token::~Token() {
    // cout << "Token Destructed." << endl;
}

TokenType Token::getType() {
    return _type;
}

string Token::getValue() {
    return _value;
}

void Token::display() {
    cout << "(" << _type << ", " << _value << ")" << endl;
}

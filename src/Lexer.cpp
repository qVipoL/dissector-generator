#include "../include/Lexer/Lexer.h"

#include "../include/util/io.h"

/* Private */

void Lexer::lexerAdvance() {
    if (_curr_char == '\n')
        _line_n += 1;

    if (_curr_char != '\0' && _char_idx < _content_length) {
        _char_idx += 1;
        _curr_char = _content[_char_idx];
    } else
        _curr_char = '\0';
}

void Lexer::skipWhitespace() {
    while (_curr_char == ' ' || _curr_char == '\n' || (int)_curr_char == '\t')
        this->lexerAdvance();
}

void Lexer::skipComment() {
    while (_curr_char != '\n')
        this->lexerAdvance();
}

Token Lexer::collectNumber() {
    string buffer;

    while (isdigit(_curr_char)) {
        buffer += _curr_char;
        this->lexerAdvance();
    }

    return Token(TOKEN_NUMBER, buffer);
}

Token Lexer::collectId() {
    string buffer;

    while (isalnum(_curr_char) || _curr_char == '_') {
        buffer += _curr_char;

        this->lexerAdvance();
    }

    return Token(TOKEN_ID, buffer);
}

Token Lexer::collectString() { /* Throws runtime exception */
    ostringstream stringStream;
    string buffer;

    this->lexerAdvance();

    while (_curr_char != '"') {
        if (_curr_char == '\0') {
            stringStream << "[Line " << _line_n << "] Expected closing \"" << endl;
            throw runtime_error(stringStream.str());
        }

        buffer += _curr_char;

        this->lexerAdvance();
    }

    this->lexerAdvance();

    return Token(TOKEN_STRING, buffer);
}

Token Lexer::advanceWithToken(TokenType type) {
    string buffer = "";
    buffer += _curr_char;

    this->lexerAdvance();

    return Token(type, buffer);
}

/* Public */

Lexer::Lexer(string content) {
    _content = content;
    _content_length = content.length();
    _char_idx = 0;
    _curr_char = _content[_char_idx];
    _line_n = 1;
}

Lexer::~Lexer() {
    // cout << "Lexer Destructed." << endl;
}

Token Lexer::getNextToken() { /* Throws runtime exception */
    ostringstream stringStream;

    while (_curr_char != '\0' && _char_idx < _content_length) {
        if (_curr_char == ' ' || _curr_char == '\n' || _curr_char == '\t')
            this->skipWhitespace();

        if (_curr_char == '#') {
            this->skipComment();
            continue;
        }

        if (isdigit(_curr_char))
            return this->collectNumber();

        if (isalnum(_curr_char))
            return this->collectId();

        if (_curr_char == '=') {
            this->lexerAdvance();

            if (_curr_char == '=') {
                this->lexerAdvance();
                return Token(TOKEN_EQUALS_EQUALS, "==");
            }

            return Token(TOKEN_EQUALS, "=");
        }

        if (_curr_char == '!') {
            this->lexerAdvance();

            if (_curr_char != '=') {
                stringStream << "[Line " << _line_n << "] Expected = after !" << endl;
                throw runtime_error(stringStream.str());
            }

            this->lexerAdvance();
            return Token(TOKEN_NOT_EQUALS, "!=");
        }

        switch (_curr_char) {
            case '"':
                return this->collectString();

            case '<':
                return this->advanceWithToken(TOKEN_LESS_THAN);

            case '>':
                return this->advanceWithToken(TOKEN_LARGER_THAN);

            case '{':
                return this->advanceWithToken(TOKEN_LBRACE);

            case '}':
                return this->advanceWithToken(TOKEN_RBRACE);

            case '[':
                return this->advanceWithToken(TOKEN_LBRACKET);

            case ']':
                return this->advanceWithToken(TOKEN_RBRACKET);

            case ':':
                return this->advanceWithToken(TOKEN_COLON);

            case ';':
                return this->advanceWithToken(TOKEN_SEMI_COLON);

            case ',':
                return this->advanceWithToken(TOKEN_COMMA);

            case '\0':
                return this->advanceWithToken(TOKEN_EOF);

            default:;
                stringStream << "[Line " << _line_n << "] Unexpected char " << _curr_char << endl;
                throw runtime_error(stringStream.str());
        }
    }

    return Token(TOKEN_EOF, "\0");
}
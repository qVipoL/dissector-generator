#include "../include/Parser/Parser.h"

/* private */

void Parser::eatToken(TokenType type) {
    ostringstream stringStream;

    if (_curr_token->getType() != type) {
        stringStream << "[Line " << _lexer->getLineN() << "] Expected type: " << type << endl;
        throw runtime_error(stringStream.str());
    }

    delete _curr_token;
    _curr_token = _lexer->getNextToken();
}

void Parser::eatToken(TokenType type, string value) {
    ostringstream stringStream;

    if (_curr_token->getType() != type || _curr_token->getValue().compare(value) != 0) {
        stringStream << "[Line " << _lexer->getLineN() << "] Expected type: " << type << " and value: " << value << endl;
        throw runtime_error(stringStream.str());
    }

    delete _curr_token;
    _curr_token = _lexer->getNextToken();
}

AST *Parser::parseProto() {
    AST *node = new AST(AST_PROTO, _lexer->getLineN());

    _curr_token = _lexer->getNextToken();

    while (_curr_token->getType() != TOKEN_EOF) {
        node->addChild(this->parseProtoDecl());
    }

    return node;
}

AST *Parser::parseProtoDecl() {
    AST *node = new AST(AST_PROTO_DECL, _lexer->getLineN());
    string value = _curr_token->getValue();
    ostringstream stringStream;

    this->eatToken(TOKEN_ID);

    if (value.compare("endian") == 0)
        node->addChild(this->parseEndian());
    else if (value.compare("details") == 0)
        node->addChild(this->parseProtoDetails());
    else if (value.compare("dissector") == 0)
        node->addChild(this->parseDissectorEntry());
    else if (value.compare("dissectorTable") == 0)
        node->addChild(this->parseDissectorTable());
    else {
        stringStream << "[Line " << node->getLineN() << "] Unexpected protocol declaration" << endl;
        throw runtime_error(stringStream.str());
    }

    this->eatToken(TOKEN_SEPARATOR, ";");

    return node;
}

AST *Parser::parseEndian() {
    AST *node = new AST(AST_ENDIAN, _lexer->getLineN());
    ostringstream stringStream;

    this->eatToken(TOKEN_OPERATION, "=");
    string endianValue = _curr_token->getValue();
    this->eatToken(TOKEN_ID);

    if (endianValue.compare("big") != 0 && endianValue.compare("little") != 0) {
        stringStream << "[Line " << node->getLineN() << "] Expected big or little endian" << endl;
        throw runtime_error(stringStream.str());
    }

    node->addId(endianValue);

    return node;
}

AST *Parser::parseProtoDetails() {
    AST *node = new AST(AST_PROTO_DETAILS, _lexer->getLineN());

    this->eatToken(TOKEN_OPERATION, "=");
    this->eatToken(TOKEN_SEPARATOR, "{");
    string protoName = _curr_token->getValue();
    this->eatToken(TOKEN_STRING);
    this->eatToken(TOKEN_SEPARATOR, ",");
    string protoDesc = _curr_token->getValue();
    this->eatToken(TOKEN_STRING);
    this->eatToken(TOKEN_SEPARATOR, ",");
    string protoAbbrv = _curr_token->getValue();
    this->eatToken(TOKEN_SEPARATOR, "}");

    node->addString(protoName);
    node->addString(protoDesc);
    node->addString(protoAbbrv);

    return node;
}

AST *Parser::parseDissectorEntry() {
    AST *node = new AST(AST_DISSECTOR_ENTRY, _lexer->getLineN());

    string dissector = _curr_token->getValue();
    this->eatToken(TOKEN_ID);
    this->eatToken(TOKEN_OPERATION, "=");
    string entry = _curr_token->getValue();
    this->eatToken(TOKEN_ID);

    node->addString(dissector);
    node->addString(entry);

    return node;
}

AST *Parser::parseDissectorTable() {
    AST *node = new AST(AST_DISSECTOR_TABLE, _lexer->getLineN());

    this->eatToken(TOKEN_SEPARATOR, "[");
    string tableType = _curr_token->getValue();
    this->eatToken(TOKEN_STRING);
    this->eatToken(TOKEN_SEPARATOR, ",");
    string tableEntry = _curr_token->getValue();
    this->eatToken(TOKEN_SEPARATOR, "]");
    this->eatToken(TOKEN_OPERATION, "=");
    string dissector = _curr_token->getValue();
    this->eatToken(TOKEN_ID);

    node->addId(dissector);

    node->addString(tableType);
    node->addString(tableEntry);

    return node;
}

/* public */

Parser::Parser(Lexer *lexer) {
    _lexer = lexer;
}

Parser::~Parser() {
    // delete _lexer;
    delete _curr_token;
}

AST *Parser::parse() {
    return this->parseProto();
}
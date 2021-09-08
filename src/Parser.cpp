#include "../include/Parser/Parser.h"

/* Private */

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
    else if (value.compare("enum") == 0)
        node->addChild(this->parseEnum());
    else if (value.compare("struct") == 0)
        node->addChild(this->parseStruct());
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
    this->eatToken(TOKEN_STRING);
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

    node->addId(dissector);
    node->addId(entry);

    return node;
}

AST *Parser::parseDissectorTable() {
    AST *node = new AST(AST_DISSECTOR_TABLE, _lexer->getLineN());

    this->eatToken(TOKEN_SEPARATOR, "[");
    string tableType = _curr_token->getValue();
    this->eatToken(TOKEN_STRING);
    this->eatToken(TOKEN_SEPARATOR, ",");
    string tableEntry = _curr_token->getValue();
    this->eatToken(TOKEN_STRING);
    this->eatToken(TOKEN_SEPARATOR, "]");
    this->eatToken(TOKEN_OPERATION, "=");
    string dissector = _curr_token->getValue();
    this->eatToken(TOKEN_ID);

    node->addId(dissector);
    node->addString(tableType);
    node->addString(tableEntry);

    return node;
}

AST *Parser::parseEnum() {
    AST *node = new AST(AST_ENUM, _lexer->getLineN());

    string enumId = _curr_token->getValue();
    this->eatToken(TOKEN_ID);
    this->eatToken(TOKEN_SEPARATOR, ":");
    string enumType = _curr_token->getValue();
    this->eatToken(TOKEN_ID);
    this->eatToken(TOKEN_SEPARATOR, "{");

    node->addChild(this->parseEnumBody());

    while (_curr_token->getValue().compare(",") == 0) {
        this->eatToken(TOKEN_SEPARATOR, ",");
        node->addChild(this->parseEnumBody());
    }

    this->eatToken(TOKEN_SEPARATOR, "}");

    return node;
}

AST *Parser::parseEnumBody() {
    AST *node;
    ostringstream stringStream;

    if (_curr_token->getType() == TOKEN_NUMBER) {
        node = new AST(AST_ENUM_BODY_DECL, _lexer->getLineN());
        string enumValue = _curr_token->getValue();
        this->eatToken(TOKEN_NUMBER);
        this->eatToken(TOKEN_SEPARATOR, "=");
        string enumId = _curr_token->getValue();
        this->eatToken(TOKEN_ID);
        this->eatToken(TOKEN_SEPARATOR, ":");
        string displayValue = _curr_token->getValue();
        this->eatToken(TOKEN_STRING);

        node->addNumber(enumValue);
        node->addId(enumId);
        node->addString(displayValue);

    } else if (_curr_token->getValue().compare("default") == 0) {
        node = new AST(AST_ENUM_BODY_DEFAULT, _lexer->getLineN());
        this->eatToken(TOKEN_ID);
        this->eatToken(TOKEN_SEPARATOR, "=");
        string displayValue = _curr_token->getValue();
        this->eatToken(TOKEN_STRING);

        node->addString(displayValue);

    } else {
        stringStream << "[Line " << node->getLineN() << "] Unexpected enum body" << endl;
        throw runtime_error(stringStream.str());
    }

    return node;
}

AST *Parser::parseStruct() {
    AST *node = new AST(AST_STRUCT, _lexer->getLineN());

    string structId = _curr_token->getValue();
    this->eatToken(TOKEN_ID);
    this->eatToken(TOKEN_SEPARATOR, "{");

    while (_curr_token->getValue().compare("}") != 0) {
        this->parseStructBody();
        this->eatToken(TOKEN_SEPARATOR, ";");
    }

    this->eatToken(TOKEN_SEPARATOR, "}");

    return node;
}

AST *Parser::parseStructBody() {
    AST *node;
    ostringstream stringStream;

    if (_curr_token->getValue().compare("switch") == 0) {
        node = new AST(AST_STRUCT_BODY_SWITCH, _lexer->getLineN());
        node->addChild(this->parseSwitch());
    } else if (_curr_token->getType() == TOKEN_ID) {
        node = new AST(AST_STRUCT_BODY_LOCAL, _lexer->getLineN());
        node->addChild(this->parseLocalElement());
    } else {
        stringStream << "[Line " << node->getLineN() << "] Unexpected struct body" << endl;
        throw runtime_error(stringStream.str());
    }

    return node;
}

AST *Parser::parseLocalElement() {
    AST *node = new AST(AST_LOCAL_ELEMENT, _lexer->getLineN());
    ostringstream stringStream;

    string elementType = _curr_token->getValue();
    this->eatToken(TOKEN_ID);
    string elementValue = _curr_token->getValue();

    node->addId(elementType);

    if (_curr_token->getType() == TOKEN_ID) {
        this->eatToken(TOKEN_ID);
        node->addId(elementValue);
    } else if (_curr_token->getType() == TOKEN_STRING) {
        this->eatToken(TOKEN_STRING);
        node->addString(elementValue);
    } else {
        stringStream << "[Line " << node->getLineN() << "] Unknown local element value type" << endl;
        throw runtime_error(stringStream.str());
    }

    return node;
}

AST *Parser::parseSwitch() {
    AST *node = new AST(AST_SWITCH, _lexer->getLineN());

    this->eatToken(TOKEN_SEPARATOR, "(");
    node->addChild(this->parseSwitchCondition());
    this->eatToken(TOKEN_SEPARATOR, ")");
    this->eatToken(TOKEN_SEPARATOR, "{");

    while (_curr_token->getValue().compare("}") != 0) {
        node->addChild(this->parseCase());
    }

    this->eatToken(TOKEN_SEPARATOR, "}");

    return node;
}

AST *Parser::parseSwitchCondition() {
    AST *node = new AST(AST_SWITCH_CONDITION, _lexer->getLineN());

    node->addChild(this->parseFieldPath());

    return node;
}

AST *Parser::parseFieldPath() {
    AST *node = new AST(AST_FIELD_PATH, _lexer->getLineN());
    ostringstream stringStream;

    string startsWith = _curr_token->getValue();

    if (startsWith.compare("../") != 0 && startsWith.compare("/") != 0) {
        stringStream << "[Line " << node->getLineN() << "] Unexpected path" << endl;
        throw runtime_error(stringStream.str());
    }

    this->eatToken(TOKEN_SEPARATOR);
    node->addStartsWith(startsWith);
    node->addChild(this->parseField());

    while (_curr_token->getValue().compare("/") != 0) {
        this->eatToken(TOKEN_SEPARATOR, "/");
        node->addChild(this->parseField());
    }

    return node;
}

AST *Parser::parseField() {
    AST *node = new AST(AST_FIELD, _lexer->getLineN());
    ostringstream stringStream;

    string field = _curr_token->getValue();

    if (_curr_token->getType() == TOKEN_ID) {
        this->eatToken(TOKEN_ID);
        node->addId(field);
    } else if (_curr_token->getType() == TOKEN_STRING) {
        this->eatToken(TOKEN_STRING);
        node->addString(field);
    } else {
        stringStream << "[Line " << node->getLineN() << "] Unexpected field type" << endl;
        throw runtime_error(stringStream.str());
    }

    return node;
}

AST *Parser::parseCase() {
    AST *node = new AST(AST_CASE, _lexer->getLineN());
    ostringstream stringStream;

    if (_curr_token->getValue().compare("case") == 0) {
        this->eatToken(TOKEN_ID);
        node->addChild(this->parseCaseLabel());
    } else if (_curr_token->getValue().compare("default") == 0) {
        this->eatToken(TOKEN_ID);
    } else {
        stringStream << "[Line " << node->getLineN() << "] Unknown case decaration" << endl;
        throw runtime_error(stringStream.str());
    }

    this->eatToken(TOKEN_SEPARATOR, ":");
    node->addChild(this->parseCaseBody());
    this->eatToken(TOKEN_SEPARATOR, ";");

    return node;
}

AST *Parser::parseCaseLabel() {
    AST *node = new AST(AST_CASE_LABEL, _lexer->getLineN());
    ostringstream stringStream;

    string labelValue = _curr_token->getValue();

    if (_curr_token->getType() == TOKEN_ID) {
        this->eatToken(TOKEN_ID);
        node->addId(labelValue);
    } else if (_curr_token->getType() == TOKEN_NUMBER) {
        this->eatToken(TOKEN_NUMBER);
        node->addNumber(labelValue);
    } else {
        stringStream << "[Line " << node->getLineN() << "] Unexpected case label type" << endl;
        throw runtime_error(stringStream.str());
    }

    return node;
}

AST *Parser::parseCaseBody() {
    AST *node = new AST(AST_CASE_BODY, _lexer->getLineN());

    string bodyValue = _curr_token->getValue();
    this->eatToken(TOKEN_ID);

    if (bodyValue.compare("void") == 0)
        node->addId(bodyValue);
    else
        node->addChild(this->parseLocalElement());

    return node;
}

/* Public */

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
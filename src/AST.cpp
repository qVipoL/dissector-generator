#include "../include/AST/AST.h"

/* public */

AST::AST(ASTType type) {
    _type = type;
    _ids = {""};
    _strings = {""};
    _numbers = {""};
    _starts_with = {""};
}

AST::AST(ASTType type, int line_n) {
    _type = type;
    _line_n = line_n;
}

AST::~AST() {
    for (AST *child : _children) {
        delete child;
    }
}

vector<AST *> AST::getChildren() {
    return _children;
}

ASTType AST::getType() {
    return _type;
}

string AST::getId(int idx) {
    if (_ids.size() == 0 || _ids.size() <= idx) return "";
    return _ids[idx];
}

string AST::getString(int idx) {
    if (_strings.size() == 0 || _strings.size() <= idx) return "";
    return _strings[idx];
}

string AST::getNumber(int idx) {
    if (_numbers.size() == 0 || _numbers.size() <= idx) return "";
    return _numbers[idx];
}

string AST::getStartsWith(int idx) {
    if (_starts_with.size() == 0 || _starts_with.size() <= idx) return "";
    return _starts_with[idx];
}

int AST::getLineN() {
    return _line_n;
}

void AST::addChild(AST *child) {
    _children.push_back(child);
}

void AST::addId(string id) {
    _ids.push_back(id);
}

void AST::addString(string string) {
    _strings.push_back(string);
}

void AST::addNumber(string number) {
    _numbers.push_back(number);
}

void AST::addStartsWith(string startsWith) {
    _starts_with.push_back(startsWith);
}

void AST::display(int width) {
    int i;
    cout << TYPES[_type] << endl;

    for (AST *child : _children) {
        for (i = 0; i < width; i++)
            cout << "  ";
        child->display(width + 1);
    }
}
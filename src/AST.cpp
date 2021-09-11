#include "../include/AST/AST.h"

/* public */

AST::AST(ASTType type) {
    _type = type;
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
    return _ids[idx];
}

string AST::getString(int idx) {
    return _strings[idx];
}

string AST::getNumber(int idx) {
    return _numbers[idx];
}

string AST::getStartsWith(int idx) {
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
#ifndef __AST_H__
#define __AST_H__

#include "../util/std_include.h"
#include "ASTType.h"

class AST {
   private:
    ASTType _type;
    int _line_n;
    vector<AST *> _children;
    vector<string> _ids, _strings, _numbers, _starts_with;

   public:
    AST(ASTType type);
    AST(ASTType type, int line_n);
    ~AST();

    ASTType getType();
    int getLineN();
    vector<AST *> getChildren();
    string getId(int idx);
    string getString(int idx);
    string getNumber(int idx);
    string getStartsWith(int idx);

    void addChild(AST *child);
    void addId(string id);
    void addString(string string);
    void addNumber(string number);
    void addStartsWith(string startWith);

    void display(int width);
};

#endif
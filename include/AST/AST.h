#ifndef __AST_H__
#define __AST_H__

#include "../util/std_include.h"
#include "ASTType.h"

class AST {
   private:
    ASTType _type;
    int _line_n;
    list<AST> _children;

    list<string> _ids;
    list<string> _strings;

   public:
    AST(ASTType type);
    AST(ASTType type, int line_n);
    ~AST();

    ASTType getType();
    list<string> getIds();
    list<string> getStrings();

    void setIds(list<string> ids);
    void setStrings(list<string> strings);
    void addChild(AST child);
};

#endif
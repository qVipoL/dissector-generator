#ifndef __AST_H__
#define __AST_H__

#include "../util/std_include.h"
#include "ASTType.h"

class AST {
   private:
    ASTType _type;
    int _line_n;
    vector<AST *> _children;

    vector<string> _ids;
    vector<string> _strings;
    vector<string> _numbers;

    vector<string> _starts_with;

    const vector<string> TYPES = {
        "AST_PROTO",
        "AST_PROTO_DECL",
        "AST_ENDIAN",
        "AST_PROTO_DETAILS",
        "AST_DISSECTOR_ENTRY",
        "AST_DISSECTOR_TABLE",
        "AST_ENUM",
        "AST_ENUM_BODY_DECL",
        "AST_ENUM_BODY_DEFAULT",
        "AST_STRUCT",
        "AST_STRUCT_BODY_LOCAL",
        "AST_STRUCT_BODY_SWITCH",
        "AST_LOCAL_ELEMENT",
        "AST_SWITCH",
        "AST_SWITCH_CONDITION",
        "AST_FIELD_PATH",
        "AST_FIELD",
        "AST_CASE",
        "AST_CASE_LABEL",
        "AST_CASE_BODY"};

   public:
    AST(ASTType type);
    AST(ASTType type, int line_n);
    ~AST();

    ASTType getType();
    string getId(int idx);
    string getString(int idx);
    string getNumber(int idx);
    string getStartsWith(int idx);
    int getLineN();

    void addChild(AST *child);
    void addId(string id);
    void addString(string string);
    void addNumber(string number);
    void addStartsWith(string startWith);

    void display(int width);
};

#endif
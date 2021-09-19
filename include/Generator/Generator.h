#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "../AST/AST.h"
#include "../util/std_include.h"
#include "Dissector/Dissector.h"
#include "Endian/EndianType.h"
#include "Enum/EnumInfo.h"
#include "Struct/FieldPath.h"
#include "Struct/StructElement.h"
#include "Struct/StructInfo.h"

class StructInfo;
class SwitchCase;

class Generator {
   private:
    AST *_tree;

    bool semantic_error;

    EndianType _endian_type;
    map<string, Dissector *> _dissectors;
    map<string, EnumInfo *> _enums;
    map<string, StructInfo *> _structs;
    StructInfo *_dissector_struct;

    EnumInfo *_curr_enum;
    StructInfo *_curr_struct;

    map<string, int> TYPE_TO_BYTE = {
        {"uint8", 1},
        {"uint16", 2},
        {"uint32", 4},
    };

    void process();
    void processNode(AST *node);
    void processProto(AST *node);
    void processProtoDecl(AST *node);
    void processEndian(AST *node);
    void processProtoDetails(AST *node);
    void processDissectorEntry(AST *node);
    void processDissectorTable(AST *node);
    void processEnum(AST *node);
    void processEnumBodyDecl(AST *node);
    void processEnumBodyDefault(AST *node);
    void processStruct(AST *node);
    void processStructChild(AST *node);
    StructElement *processLocalElement(AST *node);
    StructElement *processSwitch(AST *node);
    FieldPath *processFieldPath(AST *node);
    void processCaseBody(SwitchCase *case_element, AST *node, string label_text);

    string generateDissector(string name);
    string generateEnum(string name);
    string generateProtoFields(string name);
    string Generator::generateProtoStructs(string name);
    string generateProtoEnding(string name);

    string generate();
    bool isMissingDeclarations();
    void setupReferences();
    void setupItemReferences();
    string generateCode();

   public:
    Generator(AST *tree);
    ~Generator();

    StructInfo *getStruct(string struct_name);
    EnumInfo *getEnum(string enum_name);

    string generateLua();
};

#endif
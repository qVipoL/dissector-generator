#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "../AST/AST.h"
#include "../util/std_include.h"
#include "Dissector/Dissector.h"
#include "Endian/EndianType.h"
#include "Enum/EnumInfo.h"
#include "Struct/StructInfo.h"

class Generator {
   private:
    AST *_tree;

    EndianType _endian_type;
    map<string, Dissector *> _dissectors;
    map<string, EnumInfo *> _enums;
    map<string, StructInfo *> _structs;

    EnumInfo *_curr_enum;
    StructInfo *_curr_struct;

    const map<string, int> TYPE_TO_BYTE = {};

   public:
    Generator(AST *tree);
    ~Generator();
};

#endif
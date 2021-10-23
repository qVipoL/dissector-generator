#ifndef __LOCAL_ELEMENT_H__
#define __LOCAL_ELEMENT_H__

#include "StructElement.h"

/*
 * LocalElement
 * Usage - Concrete implementation of the abstract StructElement
 * it defines the local element of an enum (a variable)
 */
class LocalElement : public StructElement {
   public:
    LocalElement(StructElementType element_type);
    ~LocalElement();

    vector<FieldPath *> getNeeded() override;
    vector<FieldPath *> getItemNeeded() override;

    bool checkMissing(vector<StructInfo *> missing, string name) override;
    string generateLuaStructDissect(string tree, string proto_name, string field_prefix, vector<string> *structs_left) override;
    string generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                vector<string> *field_names, vector<string> *expert_names) override;
};

#endif
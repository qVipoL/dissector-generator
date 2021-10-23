#ifndef __SWITCH_ELEMENT_H__
#define __SWITCH_ELEMENT_H__

#include "StructElement.h"

/*
 * SwitchElement
 * Usage - represents the switch element
 */
class SwitchElement : public StructElement {
   public:
    SwitchElement(StructElementType element_type);
    ~SwitchElement();

    vector<FieldPath *> getNeeded() override;
    vector<FieldPath *> getItemNeeded() override;

    bool checkMissing(vector<StructInfo *> missing, string name) override;
    string generateLuaStructDissect(string tree, string proto_name, string field_prefix, vector<string> *structs_left) override;
    string generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                vector<string> *field_names, vector<string> *expert_names) override;
};

#endif
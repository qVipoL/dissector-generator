#ifndef __STRUCT_INFO_H__
#define __STRUCT_INFO_H__

#include "../../util/std_include.h"
#include "../Dissector/Dissector.h"
#include "../Generator.h"
#include "StructElement.h"

class Generator;
class StructElement;

class StructInfo {
   private:
    string _name;
    bool _is_top_level, _is_referances_setup, _is_referenced, _is_item_references_setup, _missing_checked;
    Dissector *_dissector;
    Generator *_generator;
    vector<StructElement *> _elements;
    vector<FieldPath *> _local_vars, _needed_by_below, _local_needed_by_above,
        _local_item_vars, _items_needed_by_below;

   public:
    StructInfo(string name, Generator *generator);
    ~StructInfo();

    void setIsTopLevel(bool is_top_level);
    void setDissector(Dissector *dissector);
    void addElement(StructElement *element);

    string getName();

    bool checkMissing(vector<StructInfo *> struct_stack);
    bool checkPathIsBaseType(FieldPath *path, vector<StructInfo *> struct_stack);

    StructElement *getElement(string id);
    void setNeededInChildren(FieldPath *path);
    void setNeededInChildrenRec(FieldPath *sub_path, FieldPath *path);

    bool isLocalVar(FieldPath *path);
    bool isLocalItemVar(FieldPath *path);

    void setupReferences(vector<FieldPath *> needed_paths);
    void setupItemReferences(vector<FieldPath *> needed_items);
    string generateLuaFields(string search_prefix, vector<string> *structs_left,
                             vector<string> *field_names, vector<string> *expert_names);
    string generateLuaStructDissect(string name, vector<string> *structs_left);
};

#endif

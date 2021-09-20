#ifndef __SWITCH_CASE_H__
#define __SWITCH_CASE_H__

#include "../../util/std_include.h"
#include "../Generator.h"
#include "StructElement.h"
#include "StructInfo.h"

class StructElement;
class StructInfo;
class Generator;

class SwitchCase {
   private:
    bool _is_default, _is_void;
    string _case_value, _type_name, _display_name, _local_name;
    vector<StructElement *> _elements;
    Generator *_generator;

   public:
    SwitchCase(Generator *generator);
    ~SwitchCase();

    void setIsDefault(bool is_default);
    void setIsVoid(bool is_void);
    void setCaseValue(string case_value);
    void setTypeName(string type_name);
    void setDisplayName(string display_name);
    void setLocalName(string local_name);
    void addElement(StructElement *element);

    vector<FieldPath *> getNeeded();
    vector<FieldPath *> getItemNeeded();

    bool getIsDefault();
    bool isBaseType();
    bool checkMissing(vector<StructInfo *> struct_stack, string name);
    string getType();
    string generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                vector<string> *field_names, vector<string> *expert_names);
    string generateLuaStructDissect(string tree, string cont_name, string prefix_name, string control_var,
                                    vector<string> *structs_left, bool is_first, bool is_last);
};

#endif
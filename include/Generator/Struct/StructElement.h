#ifndef __STRUCT_ELEMENT_H__
#define __STRUCT_ELEMENT_H__

#include "../../util/std_include.h"
#include "../Generator.h"
#include "FieldPath.h"
#include "StructElementType.h"
#include "StructInfo.h"
#include "SwitchCase.h"

class SwitchCase;
class Generator;
class StructInfo;

class StructElement {
   private:
    string _type, _id;
    StructElementType _element_type;
    FieldPath *_condition_path;
    Generator *_generator;
    StructInfo *_parent;
    vector<SwitchCase *> _cases;

   public:
    StructElement(StructElementType element_type);
    ~StructElement();

    void setGenerator(Generator *generator);
    void setType(string type);
    void setId(string id);
    void setParent(StructInfo *parent);
    void setConditionPath(FieldPath *condition_path);
    void addCase(SwitchCase *case_element);
    string getId();
    string getType();

    bool isStruct();
    bool isBaseType();
    bool checkMissing(vector<StructInfo *> missing, string name);
    vector<FieldPath *> getNeeded();
};

#endif
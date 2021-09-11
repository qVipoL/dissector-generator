#ifndef __STRUCT_ELEMENT_H__
#define __STRUCT_ELEMENT_H__

#include "../../util/std_include.h"
#include "FieldPath.h"
#include "StructElementType.h"
#include "SwitchCase.h"

class SwitchCase;

class StructElement {
   private:
    string _type, _id;
    StructElementType _element_type;
    FieldPath *_condition_path;
    vector<SwitchCase *> _cases;

   public:
    StructElement(StructElementType element_type);
    ~StructElement();

    void setType(string type);
    void setId(string id);
    void setConditionPath(FieldPath *condition_path);
    void addCase(SwitchCase *case_element);
};

#endif
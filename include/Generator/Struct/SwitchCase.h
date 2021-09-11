#ifndef __SWITCH_CASE_H__
#define __SWITCH_CASE_H__

#include "../../util/std_include.h"
#include "StructElement.h"

class StructElement;

class SwitchCase {
   private:
    bool _is_default, _is_void;
    string _case_value, _type_name, _display_name, _local_name;
    vector<StructElement *> _elements;

   public:
    SwitchCase();
    ~SwitchCase();

    void setIsDefault(bool is_default);
    void setIsVoid(bool is_void);
    void setCaseValue(string case_value);
    void setTypeName(string type_name);
    void setDisplayName(string display_name);
    void setLocalName(string local_name);
    void addElement(StructElement *element);
};

#endif
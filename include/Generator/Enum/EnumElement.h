#ifndef __ENUM_ELEMENT_H__
#define __ENUM_ELEMENT_H__

#include "../../util/std_include.h"

class EnumElement {
   private:
    string _id, _value, _string;

   public:
    EnumElement(string id, string value, string string);
    ~EnumElement();

    string getId();
    string getValue();
    string getString();
};

#endif
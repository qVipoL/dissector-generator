#ifndef __ENUM_ELEMENT_H__
#define __ENUM_ELEMENT_H__

#include "../../util/std_include.h"

/*
 * EnumElement
 * Usage - Represents an element of an enum, used when
 * an enum defines values
 */
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
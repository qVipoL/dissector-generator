#ifndef __STRUCT_ELEMENT_H__
#define __STRUCT_ELEMENT_H__

#include "../../util/std_include.h"
#include "StructElementType.h"

class StructElement {
   private:
    string _name;
    StructElementType _type;

   public:
    StructElement();
    ~StructElement();
};

#endif
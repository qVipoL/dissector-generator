#ifndef __STRUCT_INFO_H__
#define __STRUCT_INFO_H__

#include "../util/std_include.h"
#include "Dissector.h"
#include "StructElement.h"

class StructInfo {
   private:
    bool _is_top_level, _is_referances_setup;
    Dissector *_dissector;
    vector<StructElement *> _elements;

   public:
    StructInfo();
    ~StructInfo();
};

#endif

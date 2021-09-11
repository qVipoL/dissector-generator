#ifndef __STRUCT_INFO_H__
#define __STRUCT_INFO_H__

#include "../../util/std_include.h"
#include "../Dissector/Dissector.h"
#include "StructElement.h"

class StructInfo {
   private:
    string _name;
    bool _is_top_level, _is_referances_setup;
    Dissector *_dissector;
    vector<StructElement *> _elements;

   public:
    StructInfo(string name);
    ~StructInfo();

    void setIsTopLevel(bool is_top_level);
    void setDissector(Dissector *dissector);
    void addElement(StructElement *element);

    string getName();
};

#endif

#ifndef __ENUM_INFO_H__
#define __ENUM_INFO_H__

#include "../../util/std_include.h"
#include "EnumElement.h"

class EnumInfo {
   private:
    string _name, _type, _default_string;
    int _bytes;
    bool _has_default;
    vector<EnumElement *> _elements;

   public:
    EnumInfo(string name, string type, int bytes);
    ~EnumInfo();

    void addElement(EnumElement *element);
    void setHasDefault(bool has_default);
    void setDefaultString(string default_string);
};

#endif
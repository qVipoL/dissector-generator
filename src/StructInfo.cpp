#include "../include/Generator/Struct/StructInfo.h"

/* Public */

StructInfo::StructInfo(string name) {
    _name = name;
    _is_top_level = false;
}

StructInfo::~StructInfo() {
    for (StructElement *element : _elements)
        delete element;
}

void StructInfo::setIsTopLevel(bool is_top_level) {
    _is_top_level = is_top_level;
}

void StructInfo::setDissector(Dissector *dissector) {
    _dissector = dissector;
}

void StructInfo::addElement(StructElement *element) {
    _elements.push_back(element);
}

string StructInfo::getName() {
    return _name;
}
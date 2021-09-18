#include "../include/Generator/Enum/EnumInfo.h"

#include "../include/util/util.h"

/* Public */

EnumInfo::EnumInfo(string name, string type, int bytes) {
    _name = name;
    _type = type;
    _bytes = bytes;
    _has_default = false;
}

EnumInfo::~EnumInfo() {
    for (EnumElement *element : _elements)
        delete element;
}

void EnumInfo::addElement(EnumElement *element) {
    _elements.push_back(element);
}

void EnumInfo::setHasDefault(bool has_default) {
    _has_default = has_default;
}

void EnumInfo::setDefaultString(string default_string) {
    _default_string = default_string;
}

string EnumInfo::getType() {
    return _type;
}

bool EnumInfo::isBaseType() {
    return typeIndex(_type) > -1;
}

vector<EnumElement *> EnumInfo::getElements() {
    return _elements;
}

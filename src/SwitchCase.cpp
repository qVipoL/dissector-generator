#include "../include/Generator/Struct/SwitchCase.h"

/* Public */

SwitchCase::SwitchCase() {
    _is_default = false;
    _is_void = false;
}

SwitchCase::~SwitchCase() {
    for (StructElement *struct_element : _elements)
        delete struct_element;
}

void SwitchCase::setIsDefault(bool is_default) {
    _is_default = _is_default;
}

void SwitchCase::setIsVoid(bool is_void) {
    _is_void = is_void;
}

void SwitchCase::setCaseValue(string case_value) {
    _case_value = case_value;
}

void SwitchCase::setTypeName(string type_name) {
    _type_name = type_name;
}

void SwitchCase::setDisplayName(string display_name) {
    _display_name = display_name;
}

void SwitchCase::setLocalName(string local_name) {
    _local_name = local_name;
}

void SwitchCase::addElement(StructElement *element) {
    _elements.push_back(element);
}
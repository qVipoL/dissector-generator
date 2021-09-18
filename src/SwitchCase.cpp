#include "../include/Generator/Struct/SwitchCase.h"

#include "../include/util/util.h"

/* Public */

SwitchCase::SwitchCase(Generator *generator) {
    _generator = generator;
    _is_default = false;
    _is_void = false;
}

SwitchCase::~SwitchCase() {
    for (StructElement *struct_element : _elements)
        delete struct_element;
}

void SwitchCase::setIsDefault(bool is_default) {
    _is_default = is_default;
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

bool SwitchCase::isBaseType() {
    if (typeIndex(_type_name) > -1)
        return true;

    EnumInfo *enum_type = _generator->getEnum(_type_name);

    if (enum_type != NULL)
        return enum_type->isBaseType();

    StructInfo *struct_type = _generator->getStruct(_type_name);

    if (struct_type != NULL)
        return false;

    return true;
}

bool SwitchCase::checkMissing(vector<StructInfo *> struct_stack, string name) {
    if (_is_void)
        return false;

    if (_elements.size() > 0) {
        for (StructElement *element : _elements) {
            if (element->checkMissing(struct_stack, name))
                return true;
        }
        return false;
    }

    if (this->isBaseType())
        return false;

    return true;
}

vector<FieldPath *> SwitchCase::getNeeded() {
    vector<FieldPath *> needed;

    for (StructElement *element : _elements) {
        vector<FieldPath *> element_needed = element->getNeeded();

        for (FieldPath *path : element_needed)
            needed.push_back(path);
    }

    return needed;
}
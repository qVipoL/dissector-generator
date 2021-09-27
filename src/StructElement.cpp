#include "../include/Generator/Struct/StructElement.h"

#include "../include/util/util.h"

/* Public */

StructElement::StructElement(StructElementType element_type) {
    _element_type = element_type;
}

StructElement::~StructElement() {
    for (SwitchCase *case_element : _cases)
        delete case_element;
}

string StructElement::getId() {
    return _id;
}

string StructElement::getType() {
    return _type;
}

void StructElement::setGenerator(Generator *generator) {
    _generator = generator;
}

void StructElement::setParent(StructInfo *parent) {
    _parent = parent;
}

void StructElement::setType(string type) {
    _type = type;
}

void StructElement::setId(string id) {
    _id = id;
}

void StructElement::setConditionPath(FieldPath *condition_path) {
    _condition_path = condition_path;
}

void StructElement::addCase(SwitchCase *case_element) {
    _cases.push_back(case_element);
}

bool StructElement::isStruct() {
    return _generator->getStruct(_type) != NULL;
}

bool StructElement::isBaseType() {
    EnumInfo *enum_element;

    if (typeIndex(_type) > -1)  // If base type
        return true;

    enum_element = _generator->getEnum(_type);

    if (enum_element != NULL)  // If type of enum is base type
        return enum_element->isBaseType();

    return false;
}

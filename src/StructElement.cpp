#include "../include/Generator/Struct/StructElement.h"

/* Public */

StructElement::StructElement(StructElementType element_type) {
    _element_type = element_type;
}

StructElement::~StructElement() {
    for (SwitchCase *case_element : _cases)
        delete case_element;
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
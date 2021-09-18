#include "../include/Generator/Enum/EnumElement.h"

/* Public */

EnumElement::EnumElement(string id, string value, string string) {
    _id = id;
    _value = value;
    _string = string;
}

EnumElement::~EnumElement() {
}

string EnumElement::getId() {
    return _id;
}

string EnumElement::getValue() {
    return _value;
}

string EnumElement::getString() {
    return _string;
}
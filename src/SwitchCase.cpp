#include "../include/Generator/Struct/SwitchCase.h"

#include "../include/Generator/Struct/LocalElement.h"
#include "../include/Generator/Struct/SwitchElement.h"
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

bool SwitchCase::getIsDefault() {
    return _is_default;
}

string SwitchCase::getType() {
    EnumInfo *enum_info = _generator->getEnum(_type_name);

    if (enum_info != NULL)
        return enum_info->getType();

    return _type_name;
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

vector<FieldPath *> SwitchCase::getItemNeeded() {
    vector<FieldPath *> needed;

    for (StructElement *element : _elements) {
        vector<FieldPath *> local_needed = element->getItemNeeded();

        for (FieldPath *local : local_needed)
            needed.push_back(local);
    }

    return needed;
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

string SwitchCase::generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                        vector<string> *field_names, vector<string> *expert_names) {
    ostringstream stringStream;
    if (_is_void) return "";

    stringStream << "";

    if (_elements.size() > 0) {
        if (_case_value.compare("") == 0)
            _case_value = "null";
        string full_name = field_prefix + "_" + _case_value;
        string new_search = search_prefix + "." + _case_value;

        for (StructElement *element : _elements) {
            stringStream << element->generateLuaFieldsDef(full_name, new_search, structs_left, field_names, expert_names);
        }

        return stringStream.str();
    }

    if (this->isBaseType()) {
        string field_name = "f_" + field_prefix + _display_name;

        stringStream << "local " << field_name << " = ProtoField.";
        stringStream << getLuaType(this->getType()) << "(";
        stringStream << "\"" << search_prefix << "." << _case_value << ".";
        stringStream << _display_name << "\", ";
        stringStream << "\"" << _display_name << "\", ";
        stringStream << getLuaDisplayType(this->getType()) << endl;

        if (_generator->getEnum(_type_name) != NULL)
            stringStream << ", " << _type_name;

        stringStream << ")" << endl;
        field_names->push_back(stringStream.str());
    }

    return stringStream.str();
}

string SwitchCase::generateLuaStructDissect(string tree, string cont_name, string prefix_name, string control_var,
                                            vector<string> *structs_left, bool is_first, bool is_last) {
    ostringstream stringStream;

    if (is_first)
        stringStream << "    if " << control_var << " == " << _case_value << " then" << endl;
    else if (_is_default)
        stringStream << endl
                     << "    else " << endl;
    else
        stringStream << endl
                     << "    elseif " << control_var << " == " << _case_value << " then" << endl;

    string local_prefix_name = prefix_name + "_" + _case_value;

    if (_is_void) {
    } else if (_elements.size() > 0) {
        for (StructElement *element : _elements)
            stringStream << element->generateLuaStructDissect(tree, cont_name, local_prefix_name, structs_left);
    }

    if (is_last)
        stringStream << "    end";

    return stringStream.str();
}

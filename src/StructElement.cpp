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

string StructElement::getId() {
    return _id;
}

string StructElement::getType() {
    return _type;
}

bool StructElement::checkMissing(vector<StructInfo *> struct_stack, string name) {
    bool missing = false;
    StructInfo *struct_cont;

    if (_element_type == TYPE_LOCAL_ELEMENT) {
        if (this->isStruct()) {
            struct_cont = _generator->getStruct(_type);

            return struct_cont->checkMissing(struct_stack);
        }

        if (this->isBaseType())
            return false;

        return true;
    } else {
        if (!_parent->checkPathIsBaseType(_condition_path, struct_stack))
            missing = true;

        for (SwitchCase *case_decl : _cases) {
            if (case_decl->checkMissing(struct_stack, name)) {
                missing = true;
                break;
            }
        }

        return missing;
    }
}

vector<FieldPath *> StructElement::getNeeded() {
    vector<FieldPath *> needed;
    StructInfo *struct_element;

    if (_element_type == TYPE_SWITCH) {  // Switch
        needed.push_back(_condition_path);

        for (SwitchCase *case_element : _cases) {
            vector<FieldPath *> case_needed = case_element->getNeeded();

            for (FieldPath *need : case_needed)
                needed.push_back(need);
        }
    } else if (this->isStruct()) {  // If local element type is a struct
        StructInfo *struct_element = _generator->getStruct(_type);
        vector<FieldPath *> struct_need;
        struct_element->setupReferences(struct_need);

        return struct_need;
    }

    return needed;
}

vector<FieldPath *> StructElement::getItemNeeded() {
    vector<FieldPath *> needed;

    if (_element_type == TYPE_LOCAL_ELEMENT) {
        if (this->isStruct()) {
            StructInfo *cur_struct = _generator->getStruct(_type);
            cur_struct->setupItemReferences(needed);
        }
    } else {
        for (SwitchCase *case_element : _cases) {
            vector<FieldPath *> items = case_element->getItemNeeded();

            for (FieldPath *item : items)
                needed.push_back(item);
        }
    }

    return needed;
}

string StructElement::generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                           vector<string> *field_names, vector<string> *expert_names) {
    ostringstream stringStream;

    if (_element_type == TYPE_LOCAL_ELEMENT) {
        if (this->isStruct()) {
            StructInfo *struct_info = _generator->getStruct(_type);
            string new_search_prefix = field_prefix + "." + _id;

            stringStream << struct_info->generateLuaFields(new_search_prefix, structs_left, field_names, expert_names);
        } else {
            string type;

            string field_name = "f_" + field_prefix + "_" + _id;

            if (_generator->getEnum(_type))
                type = _generator->getEnum(_type)->getType();
            else
                type = _type;

            stringStream << "local " << field_name << " = ProtoField.";
            stringStream << getLuaType(type) << "(";
            stringStream << "\"" << search_prefix << "." << _id << "\"";
            stringStream << ", \"" << _id << "\", " << getLuaDisplayType(type);

            if (_generator->getEnum(_type))
                stringStream << ", " << _type;

            stringStream << ")" << endl;

            field_names->push_back(field_name);
        }
    } else {
        for (SwitchCase *case_element : _cases) {
            stringStream << case_element->generateLuaFieldsDef(field_prefix, search_prefix, structs_left, field_names, expert_names);
        }
    }

    return stringStream.str();
}
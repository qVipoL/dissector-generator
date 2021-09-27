#include "../include/Generator/Struct/SwitchElement.h"

#include "../include/util/util.h"

/* Public */

SwitchElement::SwitchElement(StructElementType element_type) : StructElement(element_type) {
}

SwitchElement::~SwitchElement() {
}

vector<FieldPath *> SwitchElement::getNeeded() {
    vector<FieldPath *> needed;
    needed.push_back(_condition_path);

    for (SwitchCase *case_element : _cases) {
        vector<FieldPath *> case_needed = case_element->getNeeded();

        for (FieldPath *need : case_needed)
            needed.push_back(need);
    }

    return needed;
}

vector<FieldPath *> SwitchElement::getItemNeeded() {
    vector<FieldPath *> needed;

    for (SwitchCase *case_element : _cases) {
        vector<FieldPath *> items = case_element->getItemNeeded();

        for (FieldPath *item : items)
            needed.push_back(item);
    }

    return needed;
}

bool SwitchElement::checkMissing(vector<StructInfo *> struct_stack, string name) {
    bool missing = false;

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

string SwitchElement::generateLuaStructDissect(string tree, string proto_name, string field_prefix, vector<string> *structs_left) {
    ostringstream stringStream;

    string control_var = _condition_path->getParamName();
    bool is_first = true;
    SwitchCase *default_case = NULL;

    for (SwitchCase *case_element : _cases) {
        if (case_element->getIsDefault())
            default_case = case_element;
        else {
            bool is_last = false;

            for (SwitchCase *tmp : _cases)
                if (tmp == case_element)
                    is_last = true;

            stringStream << case_element->generateLuaStructDissect(tree, proto_name, field_prefix, control_var, structs_left, is_first, is_last);

            if (is_first)
                is_first = false;
        }
    }

    if (default_case != NULL)
        stringStream << default_case->generateLuaStructDissect(tree, proto_name, field_prefix, "", structs_left, false, true);

    stringStream << endl;

    return stringStream.str();
}

string SwitchElement::generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                           vector<string> *field_names, vector<string> *expert_names) {
    ostringstream stringStream;

    for (SwitchCase *case_element : _cases)
        stringStream << case_element->generateLuaFieldsDef(field_prefix, search_prefix, structs_left, field_names, expert_names);

    return stringStream.str();
}
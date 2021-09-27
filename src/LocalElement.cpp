#include "../include/Generator/Struct/LocalElement.h"

#include "../include/util/util.h"

/* Public */

LocalElement::LocalElement(StructElementType element_type) : StructElement(element_type) {
}

LocalElement::~LocalElement() {
}

vector<FieldPath *> LocalElement::getNeeded() {
    vector<FieldPath *> needed;

    if (this->isStruct()) {
        StructInfo *struct_element = _generator->getStruct(_type);
        vector<FieldPath *> struct_need;
        struct_element->setupReferences(struct_need);
        return struct_need;
    }

    return needed;
}

vector<FieldPath *> LocalElement::getItemNeeded() {
    vector<FieldPath *> needed;

    if (this->isStruct()) {
        StructInfo *cur_struct = _generator->getStruct(_type);
        cur_struct->setupItemReferences(needed);
    }

    return needed;
}

bool LocalElement::checkMissing(vector<StructInfo *> struct_stack, string name) {
    bool missing = false;
    StructInfo *struct_cont;

    if (this->isStruct()) {
        struct_cont = _generator->getStruct(_type);

        return struct_cont->checkMissing(struct_stack);
    }

    if (this->isBaseType())
        return false;

    return true;
}

string LocalElement::generateLuaStructDissect(string tree, string proto_name, string field_prefix, vector<string> *structs_left) {
    ostringstream stringStream;

    if (this->isStruct()) {
        StructInfo *struct_info = _generator->getStruct(_type);
        proto_name = "_" + _id;
        stringStream << struct_info->generateLuaDissectCall("    ", tree, "\"" + _id + "\"");
        structs_left->push_back(_type);
    } else if (this->isBaseType()) {
        StructInfo *struct_info = _generator->getStruct(proto_name);

        if (struct_info->isLocalVar(_id)) {
            FieldPath *path = struct_info->getLocalVar(_id);
            string type;

            if (_generator->getEnum(_type))
                type = _generator->getEnum(_type)->getType();
            else
                type = _type;

            stringStream << "    local l_" << path->getParamName() << " = buffer(offset, ";
            stringStream << baseTypeLenString(type) << "):uint()" << endl;
        } else if (struct_info->isNeededVar(_id)) {
            string type;

            if (_generator->getEnum(_type))
                type = _generator->getEnum(_type)->getType();
            else
                type = _type;

            FieldPath *path = struct_info->getNeeded(_id);
            stringStream << "    local l_" << path->getParamName() << " = buffer(offset, ";
            stringStream << baseTypeLenString(type) << "):uint()" << endl;
        }

        string type;

        if (_generator->getEnum(_type))
            type = _generator->getEnum(_type)->getType();
        else
            type = _type;

        stringStream << "    t_" << proto_name << ":" << getLuaAdder(type, _generator->getEndianType());
        stringStream << "(f_" << field_prefix << "_" << _id;
        stringStream << ", buffer(offset, " << baseTypeLenString(type) << "))" << endl;

        stringStream << "    offset = offset + " << baseTypeLenString(type) << endl;
    }

    return stringStream.str();
}

string LocalElement::generateLuaFieldsDef(string field_prefix, string search_prefix, vector<string> *structs_left,
                                          vector<string> *field_names, vector<string> *expert_names) {
    ostringstream stringStream;

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

    return stringStream.str();
}
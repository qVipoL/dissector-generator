#include "../include/Generator/Struct/StructInfo.h"

#include "../include/util/util.h"

/* Public */

StructInfo::StructInfo(string name, Generator *generator) {
    _generator = generator;
    _name = name;
    _is_top_level = false;
    _is_referances_setup = false;
}

StructInfo::~StructInfo() {
    for (StructElement *element : _elements)
        delete element;
}

void StructInfo::setIsTopLevel(bool is_top_level) {
    _is_top_level = is_top_level;
}

void StructInfo::setDissector(Dissector *dissector) {
    _dissector = dissector;
}

void StructInfo::addElement(StructElement *element) {
    _elements.push_back(element);
}

string StructInfo::getName() {
    return _name;
}

bool StructInfo::checkMissing(vector<StructInfo *> struct_stack) {
    bool missing = false;

    _is_referenced = true;

    if (_missing_checked == true)
        return false;

    _missing_checked = true;

    struct_stack.push_back(this);

    for (StructElement *element : _elements) {
        if (element->checkMissing(struct_stack, _name)) {
            missing = true;
            break;
        }
    }

    struct_stack.pop_back();

    return missing;
}

StructElement *StructInfo::getElement(string id) {
    for (StructElement *element : _elements) {
        if (element->getId().compare(id) == 0)
            return element;
    }

    return NULL;
}

bool StructInfo::checkPathIsBaseType(FieldPath *path, vector<StructInfo *> struct_stack) {
    StructElement *element;
    FieldPath *temp = path;
    string type;
    StructInfo *curr_struct = this;
    EnumInfo *enum_element;
    int idx;

    if (path == NULL || path->getComponentName() == "")
        return false;

    while (temp != NULL) {
        switch (temp->getPathType()) {
            case RELATIVE_CURRENT:
                element = curr_struct->getElement(temp->getComponentName());
                break;

            case RELATIVE_PARENT:
                StructInfo *local_parent = struct_stack[struct_stack.size() - 2];
                element = local_parent->getElement(temp->getComponentName());
                break;
        }

        if (element == NULL)
            return false;

        if (temp->getNext() == NULL) {  // reached the final declaration
            if (!element->isBaseType())
                return false;

            type = element->getType();
            temp->setType(type);

            idx = typeIndex(type);

            if (idx == -1) {
                enum_element = _generator->getEnum(type);

                if (enum_element != NULL)
                    idx = typeIndex(enum_element->getType());
            }
        } else {
            if (!element->isStruct())
                return false;

            curr_struct = _generator->getStruct(element->getType());

            if (curr_struct == NULL) return false;
        }

        temp = temp->getNext();
    }

    path->setType(type);

    return true;
}

void StructInfo::setNeededInChildren(FieldPath *path) {
    if (path->isOneLevel()) return;

    StructElement *element = this->getElement(path->getComponentName());

    StructInfo *struct_info = _generator->getStruct(element->getType());

    if (struct_info != NULL)
        struct_info->setNeededInChildrenRec(path->getNext(), path);
}

void StructInfo::setNeededInChildrenRec(FieldPath *sub_path, FieldPath *path) {
    if (sub_path->isOneLevel()) {
        path->addIfNotContains(&_local_needed_by_above);

        return;
    }

    StructElement *element = this->getElement(path->getComponentName());

    StructInfo *struct_info = _generator->getStruct(element->getType());

    struct_info->setNeededInChildrenRec(sub_path->getNext(), path);

    path->addIfNotContains(&_local_needed_by_above);
}

void StructInfo::setupReferences(vector<FieldPath *> needed_paths) {
    if (_is_referances_setup) return;

    _is_referances_setup = true;

    for (StructElement *element : _elements) {
        vector<FieldPath *> need = element->getNeeded();

        for (FieldPath *path : need) {
            path->addIfNotContains(&needed_paths);
        }
    }

    vector<FieldPath *> to_remove;

    for (FieldPath *path : needed_paths) {
        if (path->getPathType() != RELATIVE_PARENT) {
            path->addIfNotContains(&_local_vars);

            if (!path->isOneLevel()) {
                path->addIfNotContains(&_needed_by_below);
                this->setNeededInChildren(path);
            }

            to_remove.push_back(path);
        } else {
            path->removeRelativeParent();
            path->addIfNotContains(&_needed_by_below);
        }
    }

    for (FieldPath *path : to_remove) {
        for (int i = 0; i < needed_paths.size(); i++) {
            if (needed_paths[i] == path) {
                needed_paths.erase(next(needed_paths.begin(), i));
                break;
            }
        }
    }
}

void StructInfo::setupItemReferences(vector<FieldPath *> needed_items) {
    if (_is_item_references_setup) return;

    _is_item_references_setup = true;

    for (StructElement *element : _elements) {
        vector<FieldPath *> need = element->getItemNeeded();

        for (FieldPath *path : need) {
            path->addIfNotContains(&needed_items);
        }
    }

    vector<FieldPath *> to_remove;

    for (FieldPath *path : needed_items) {
        if (path->getPathType() != RELATIVE_PARENT) {
            path->addIfNotContains(&_local_item_vars);

            if (!path->isOneLevel()) {
                path->addIfNotContains(&_items_needed_by_below);
                this->setNeededInChildren(path);
            }

            to_remove.push_back(path);
        } else {
            path->removeRelativeParent();
            path->addIfNotContains(&_items_needed_by_below);
        }
    }

    for (FieldPath *path : to_remove) {
        for (int i = 0; i < needed_items.size(); i++) {
            if (needed_items[i] == path) {
                needed_items.erase(next(needed_items.begin(), i));
                break;
            }
        }
    }
}

string StructInfo::generateLuaFields(string search_prefix, vector<string> *structs_left,
                                     vector<string> *field_names, vector<string> *expert_names) {
    string prefix = search_prefix + "." + _name;
    ostringstream stringStream;

    stringStream << "";

    for (StructElement *element : _elements) {
        if (element->isStruct()) {
            structs_left->push_back(element->getType());
        } else {
            stringStream << element->generateLuaFieldsDef(_name, prefix, structs_left, field_names, expert_names);
        }
    }

    return stringStream.str();
}

bool StructInfo::isLocalVar(FieldPath *path) {
    for (FieldPath *temp : _local_vars)
        if (temp->equals(path))
            return true;

    return false;
}

bool StructInfo::isLocalItemVar(FieldPath *path) {
    for (FieldPath *temp : _local_item_vars)
        if (temp->equals(path))
            return true;

    return false;
}

string StructInfo::generateLuaStructDissect(string name, vector<string> *structs_left) {
    ostringstream stringStream;
    if (!_is_referenced) return;

    if (_is_top_level) {
        stringStream << "function " << name << "_proto.dissector(buffer, pinfo, tree)" << endl
                     << endl;
        stringStream << "    local offset = 0" << endl;
        stringStream << "    pinfo.cols['protocol'] = "
                     << "\"" << name << "\"";
    } else {
        stringStream << "function dissect_" << name << "(buffer, pinfo, tree";

        for (FieldPath *path : _needed_by_below)
            if (!this->isLocalVar(path))
                stringStream << ", l_" << path->getParamName();

        for (FieldPath *path : _items_needed_by_below)
            if (!this->isLocalItemVar(path))
                stringStream << ", i_" << path->getParamName();

        stringStream << ")" << endl
                     << endl;
        stringStream << "    local saved_offset = offset";

        for (FieldPath *path : _local_needed_by_above) {
            stringStream << "    local l_" << path->getParamName();
        }
    }

    string tree = "t_" + name;

    if (_is_top_level) {
        stringStream << "    local " << tree << " = tree:add(";
        stringStream << name << "_, buffer())" << endl;
    } else {
        stringStream << "    local " << tree << " = tree:add(buffer(offset, 1), label)" << endl;
    }

    for (StructElement *element : _elements) {
        if (element->isStruct()) {
            StructInfo *struct_info = _generator->getStruct(element->getType());

            // stringStream << struct_info->generateLuaDissectCall("    ", tree, "\"" + name + "\"");
            structs_left->push_back(struct_info->getName());
        } else {
            // stringStream << element->generateLuaStructDissect(tree, _name, _name, structs_left);
        }
    }

    if (!_is_top_level) {
        stringStream << "    t_" << name << ":set_len(offset - saved_offset)" << endl;
        stringStream << "    return offset";

        for (FieldPath *path : _local_needed_by_above) {
            stringStream << ", l_" << path->getParamName();
        }

        stringStream << endl;
    }

    stringStream << "end" << endl
                 << endl;

    return stringStream.str();
}

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

#include "../include/Generator/Struct/FieldPath.h"

/* Public */

FieldPath::FieldPath() {
    _path_type = RELATIVE_CURRENT;
    _next = NULL;
}

FieldPath::FieldPath(AST *node) {
    FieldPath *comp = NULL, *tmpComp = NULL;

    _path_type = RELATIVE_CURRENT;
    _next = NULL;

    if (node->getStartsWith(0) == "../")
        _path_type = ABS;
    else if (node->getStartsWith(0) == "/")
        _path_type = RELATIVE_PARENT;

    for (AST *child : node->getChildren()) {
        if (comp == NULL) {
            comp = this;
            tmpComp = comp;
        } else {
            tmpComp = new FieldPath();
            comp->setNext(tmpComp);
            comp = tmpComp;
        }

        if (child->getId(0) != "") {
            tmpComp->setComponentName(child->getId(0));
        } else {
            tmpComp->setComponentName(child->getString(0));
        }
    }
}

FieldPath::~FieldPath() {
    FieldPath *curr = _next, *prev = NULL;

    while (curr != NULL) {
        prev = curr;
        curr = curr->getNext();
        delete prev;
    }
}

void FieldPath::setType(string type) {
    _type = type;
}

void FieldPath::setNext(FieldPath *next) {
    _next = next;
}

void FieldPath::setComponentName(string component_name) {
    _component_name = component_name;
}

string FieldPath::getComponentName() {
    return _component_name;
}

FieldPath *FieldPath::getNext() {
    return _next;
}

PathType FieldPath::getPathType() {
    return _path_type;
}

bool FieldPath::equals(FieldPath *path) {
    if (path == NULL) return false;

    if (_path_type != path->getPathType()) return false;

    FieldPath *temp = this, *temp2 = path;

    while (temp != NULL) {
        if (temp->getComponentName().compare(temp2->getComponentName()) != 0)
            return false;

        temp = temp->getNext();
        temp2 = temp2->getNext();

        if (temp != NULL || temp2 == NULL)
            return false;
    }

    return temp2 != NULL;
}

bool FieldPath::isOneLevel() {
    return (_path_type != RELATIVE_PARENT && _next == NULL);
}

void FieldPath::addIfNotContains(vector<FieldPath *> *list) {
    if (list == NULL) return;

    for (FieldPath *path : (*list)) {
        if (path->equals(this)) {
            return;
        }
    }

    list->push_back(this);
}

void FieldPath::removeRelativeParent() {
    _path_type = RELATIVE_PARENT;
}

string FieldPath::getParamName() {
    FieldPath *comp = this;
    string name;

    while (comp->getNext() != NULL) {
        if (name.compare("") == 0)
            name = comp->getComponentName();
        else
            name = name + "_" + comp->getComponentName();

        comp = comp->getNext();
    }

    return name.compare("") == 0 ? comp->getComponentName() : name + "_" + comp->getComponentName();
}

bool FieldPath::equals(string name) {
    if (_next != NULL) return false;

    if (_path_type != RELATIVE_CURRENT) return false;

    return name.compare(_component_name) == 0;
}

bool FieldPath::equalsLast(string name) {
    FieldPath *last = this;

    while (last->getNext() != NULL)
        last = last->getNext();

    return name.compare(last->getComponentName()) == 0;
}

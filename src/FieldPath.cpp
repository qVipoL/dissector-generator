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

void FieldPath::setNext(FieldPath *next) {
    _next = next;
}

void FieldPath::setComponentName(string component_name) {
    _component_name = component_name;
}

FieldPath *FieldPath::getNext() {
    return _next;
}
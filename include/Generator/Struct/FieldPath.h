#ifndef __FIELD_PATH_H__
#define __FIELD_PATH_H__

#include "../../AST/AST.h"
#include "../../util/std_include.h"
#include "PathType.h"

class FieldPath {
   private:
    PathType _path_type;
    FieldPath *_next;
    string _component_name;

   public:
    FieldPath();
    FieldPath(AST *node);
    ~FieldPath();

    void setNext(FieldPath *next);
    void setComponentName(string component_name);

    FieldPath *getNext();
};

#endif
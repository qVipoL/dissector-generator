#ifndef __FIELD_PATH_H__
#define __FIELD_PATH_H__

#include "../../AST/AST.h"
#include "../../util/std_include.h"
#include "PathType.h"

class FieldPath {
   private:
    PathType _path_type;
    string _type, _component_name;
    FieldPath *_next;

   public:
    FieldPath();
    FieldPath(AST *node);
    ~FieldPath();

    PathType getPathType();
    string getComponentName();
    FieldPath *getNext();
    string getParamName();

    void setType(string type);
    void setNext(FieldPath *next);
    void setComponentName(string component_name);

    void addIfNotContains(vector<FieldPath *> *list);

    bool isOneLevel();
    void removeRelativeParent();

    bool equals(FieldPath *path);
    bool equals(string name);
    bool equalsLast(string name);
};

#endif
#ifndef __FIELD_PATH_H__
#define __FIELD_PATH_H__

#include "../../AST/AST.h"
#include "../../util/std_include.h"
#include "PathType.h"

class FieldPath {
   private:
    PathType _path_type;
    string _type;
    FieldPath *_next;
    string _component_name;

   public:
    FieldPath();
    FieldPath(AST *node);
    ~FieldPath();

    void setType(string type);
    void setNext(FieldPath *next);
    void setComponentName(string component_name);
    string getComponentName();

    FieldPath *getNext();
    PathType getPathType();
    string getParamName();

    void addIfNotContains(vector<FieldPath *> *list);
    bool equals(FieldPath *path);
    bool isOneLevel();
    void removeRelativeParent();
};

#endif
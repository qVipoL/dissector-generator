#ifndef __PATH_TYPE_H__
#define __PATH_TYPE_H__

/*
 * PathType
 * Usage - all the possible path types
 * abs - absolute type (global)
 * relative_parent - relative to the parent struct
 * relative_current - relative to the current struct
 */
enum PathType {
    ABS,
    RELATIVE_PARENT,
    RELATIVE_CURRENT
};

#endif
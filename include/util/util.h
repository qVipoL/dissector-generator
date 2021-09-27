#ifndef __UTIL_H__
#define __UTIL_H__

#include "../Generator/Endian/EndianType.h"
#include "std_include.h"

int typeIndex(string type);
int baseTypeLen(string type);

string getLuaType(string type);
string getLuaDisplayType(string type);
string getLuaAdder(string type, EndianType endian);
string baseTypeLenString(string type);

#endif
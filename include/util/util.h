#ifndef __UTIL_H__
#define __UTIL_H__

#include "../Generator/Endian/EndianType.h"
#include "std_include.h"

/*
 * Utility functions for the generator
 */

int typeIndex(string type);
int baseTypeLen(string type);

string getLuaType(string type);
string getLuaDisplayType(string type);
string getLuaAdder(string type, EndianType endian);
string baseTypeLenString(string type);

string decToHex(string dec);

#endif
#include "../include/util/util.h"

static vector<string> BASE_TYPES = {
    "uint8", "uint16", "uint32", "uint64"};

static vector<string> LUA_TYPES = {
    "uint8", "uint16", "uint32", "uint64"};

static vector<string> FIELD_BASE = {
    "BASE.DEC", "BASE.DEC", "BASE.DEC", "BASE.DEC"};

static vector<int> BASE_TYPE_LENS = {
    1, 2, 4, 8};

static vector<string> LE_LUA_ADDER = {
    "add_le", "add_le", "add_le", "add_le"};

static vector<string> BE_LUA_ADDER = {
    "add", "add", "add", "add"};

int typeIndex(string type) {
    int i;

    for (i = 0; i < BASE_TYPES.size(); i++) {
        if (BASE_TYPES[i].compare(type) == 0)
            return i;
    }

    return -1;
}

string getLuaType(string type) {
    int idx = typeIndex(type);

    return LUA_TYPES[idx];
}

string getLuaDisplayType(string type) {
    int idx = typeIndex(type);

    if (idx == -1)
        return "base.UNKNOWN";

    return FIELD_BASE[idx];
}

int baseTypeLen(string type) {
    int i;

    for (i = 0; i < BASE_TYPES.size(); i++) {
        if (BASE_TYPES[i].compare(type) == 0)
            return BASE_TYPE_LENS[i];
    }

    return -1;
}

string baseTypeLenString(string type) {
    int len = baseTypeLen(type);
    ostringstream stringStream;

    if (len < 0)
        stringStream << "Unknown";
    else
        stringStream << len;

    return stringStream.str();
}

string getLuaAdder(string type, EndianType endian) {
    int i;

    for (i = 0; i < BASE_TYPES.size(); i++) {
        if (BASE_TYPES[i].compare(type) == 0)
            if (endian == LITTLE_ENDIAN)
                return LE_LUA_ADDER[i];
            else
                return BE_LUA_ADDER[i];
    }

    return "add_UNKNOWN";
}
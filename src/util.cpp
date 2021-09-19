#include "../include/util/util.h"

static vector<string> BASE_TYPES = {
    "uint8", "uint16", "uint32", "uint64"};

static vector<string> LUA_TYPES = {
    "uint8", "uint16", "uint32", "uint64"};

static vector<string> FIELD_BASE = {
    "BASE.DEC", "BASE.DEC", "BASE.DEC", "BASE.DEC"};

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
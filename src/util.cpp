#include "../include/util/util.h"

static vector<string> BASE_TYPES = {
    "uint8", "uint16", "uint32", "uint64"};

int typeIndex(string type) {
    int i;

    for (i = 0; i < BASE_TYPES.size(); i++) {
        if (BASE_TYPES[i].compare(type) == 0)
            return i;
    }

    return -1;
}
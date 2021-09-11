#include "../include/Generator/Dissector/Dissector.h"

/* Public */

Dissector::Dissector(string details) {
    _details = details;
}

Dissector::~Dissector() {
}

void Dissector::setTable(string table) {
    _table = table;
}

void Dissector::setTableEntry(string table_entry) {
    _table_entry = table_entry;
}
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

void Dissector::setStructEntry(string struct_entry) {
    _struct_entry = struct_entry;
}

string Dissector::getDetails() {
    return _details;
}

string Dissector::getTable() {
    return _table;
}

string Dissector::getTableEntry() {
    return _table_entry;
}

string Dissector::getStructEntry() {
    return _struct_entry;
}

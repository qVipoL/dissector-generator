#ifndef __DISSECTOR_H__
#define __DISSECTOR_H__

#include "../../util/std_include.h"

/*
 * Dissector
 * Usage - Represents the dissector to be generated
 */
class Dissector {
   private:
    string _details, _table, _table_entry, _struct_entry;

   public:
    Dissector(string details);
    ~Dissector();

    string getDetails();
    string getTable();
    string getTableEntry();
    string getStructEntry();

    void setTable(string table);
    void setTableEntry(string table_entry);
    void setStructEntry(string struct_entry);
};

#endif
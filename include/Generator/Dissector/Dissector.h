#ifndef __DISSECTOR_H__
#define __DISSECTOR_H__

#include "../../util/std_include.h"

class Dissector {
   private:
    string _details, _table, _table_entry, _struct_entry;

   public:
    Dissector(string details);
    ~Dissector();

    void setTable(string table);
    void setTableEntry(string table_entry);
    void setStructEntry(string struct_entry);

    string getDetails();
    string getStructEntry();
};

#endif
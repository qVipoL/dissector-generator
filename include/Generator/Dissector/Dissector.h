#ifndef __DISSECTOR_H__
#define __DISSECTOR_H__

#include "../../util/std_include.h"

class Dissector {
   private:
    string _description, _table, _table_entry;

   public:
    Dissector();
    ~Dissector();
};

#endif
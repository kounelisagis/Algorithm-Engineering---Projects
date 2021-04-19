#ifndef AUX_FUN_H
#define AUX_FUN_H

#include "LEDA/graph/ugraph.h"
#include "LEDA/core/list.h"
#include <iostream>

using namespace leda;


void my_print_graph(const ugraph&);
void my_print_lists(const ugraph&, const list<node>&, const list<node>&);

#endif

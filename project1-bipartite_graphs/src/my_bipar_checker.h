#ifndef BIPAR_CHECK_H
#define BIPAR_CHECK_H

#include "LEDA/graph/ugraph.h"
#include "LEDA/core/queue.h"

using namespace leda;


enum Color { green, blue };

list<node> my_BFS(const ugraph&, node, node_array<int>&, node_array<edge>&, node_array<Color>&);
bool my_bipar_checker(const ugraph&, list<node>&, list<node>&);

#endif

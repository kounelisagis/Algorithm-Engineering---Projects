#ifndef GRAPH_GEN_H
#define GRAPH_GEN_H

#include "LEDA/graph/ugraph.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/array.h"

using namespace leda;


ugraph nested_sqrs(const int);
ugraph ring_graph(const int);
ugraph four_lvl_graph(const int);

#endif

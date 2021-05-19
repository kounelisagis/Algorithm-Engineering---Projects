#ifndef GRAPHS_H
#define GRAPHS_H


#include "basics.hpp"


void write_graph(Graph&);
Graph get_grid_graph(const int, const int, const int, const int);
Graph random_graph(const int, const int, const int, const int);
Graph my_reverse_graph(const Graph&);
void random_s_t_grid(const Graph&, Vertex&, Vertex&, const int, const int);
void random_s_t_random(const Graph&, Vertex&, Vertex&);


#endif


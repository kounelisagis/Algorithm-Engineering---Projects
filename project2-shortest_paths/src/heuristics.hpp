#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "basics.hpp"


std::vector<double> heuristic_euclidean(const Graph&, const Vertex&);
std::vector<double> find_new_bounds(const Graph&, const Vertex&, const Vertex&, std::vector<double>&);
Vertex pick_landmark(const Graph&, const std::vector<Vertex>, const Vertex&, const std::vector<double>&);
std::vector<double> heuristic_landmarks(const Graph&, const Vertex&, int);


#endif


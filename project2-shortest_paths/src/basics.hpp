#ifndef BASICS_H
#define BASICS_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <utility> // pair


struct NodeInfo {
	// int id;
	int x;
	int y;
};

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS, NodeInfo, property<edge_weight_t, double> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef std::pair<int,int> E;

#endif


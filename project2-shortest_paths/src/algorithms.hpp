#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "basics.hpp"
#include <boost/graph/dijkstra_shortest_paths.hpp>


using namespace boost;


// Visitor that throw an exception when finishing the destination vertex
class my_visitor : public dijkstra_visitor<> {
protected:
	Vertex destination_vertex;
	int examined_vertexes;
public:
	my_visitor(Vertex);
	void examine_vertex(const Vertex&, const Graph&);
};

int DijkstraSP(const Graph&, const Vertex, const Vertex, std::vector<double>&);
int A_star(Graph&, const Vertex, const Vertex, std::vector<double>&, const std::vector<double>&);


#endif


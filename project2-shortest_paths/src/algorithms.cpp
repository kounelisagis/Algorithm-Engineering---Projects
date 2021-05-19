#include "algorithms.hpp"

#include <boost/graph/copy.hpp>
#include <boost/graph/graph_traits.hpp>

#include <iostream>


my_visitor::my_visitor(Vertex my_destination_vertex) : destination_vertex(my_destination_vertex), examined_vertexes(0) {};


// check if t node is reached
void my_visitor::examine_vertex(const Vertex& u, const Graph& G) {
	examined_vertexes++;
	if (u == destination_vertex)
		throw(examined_vertexes);
}


int DijkstraSP(const Graph& G, const Vertex s, const Vertex t, std::vector<double>& distance_map_p) {

	// Visitor to throw an exception when the end is reached
	my_visitor vis(t);

	try {
		dijkstra_shortest_paths(
			G,
			s,
			distance_map(make_iterator_property_map(distance_map_p.begin(), get(vertex_index, G))).
			visitor(vis)
		);
		return num_vertices(G);
	}
	catch (int examined_vertexes) {  // The Dijkstra algorithm stopped
		return examined_vertexes;
	}

}


int A_star(Graph& H, const Vertex s, const Vertex t, std::vector<double>& distance_map_p, const std::vector<double>& h) {

	graph_traits<Graph>::edge_iterator ei, ei_end;

	// New weights
	for (tie(ei, ei_end) = edges(H); ei != ei_end; ++ei) {
		Vertex i = source(*ei, H);
		Vertex j = target(*ei, H);
		double weightToAdd = h[j] - h[i];
		double weight = get(edge_weight_t(), H, *ei);
		put(edge_weight_t(), H, *ei, weight + weightToAdd);
	}

	return DijkstraSP(H, s, t, distance_map_p);

}


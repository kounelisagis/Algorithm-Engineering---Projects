#include "graphs.hpp"

#include <boost/graph/copy.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/graph/random.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


// graphviz format print
void write_graph(Graph& G) {
	dynamic_properties dp;
	dp.property("node_id", get(vertex_index, G));
	dp.property("weight", get(edge_weight, G));
	write_graphviz_dp(std::cout, G, dp);
}


Graph get_grid_graph(const int r, const int c, const int weight_min, const int weight_max) {

	const int num_nodes = r*c;
	const int num_edges = 2*( (c-1)*r + (r-1)*c );

	E G_edges[num_edges];
	double G_weights[num_edges];


	// fill by column
	int vertex_counter = 0;
	int edge_counter = 0;
	for(int col=0;col<c;col++) {
		for(int row=0;row<r;row++) {
			if(row<r-1) {  // except the last row
				int weight = rand()%weight_max + weight_min;
				G_edges[edge_counter] = E(vertex_counter, vertex_counter+1);
				G_weights[edge_counter++] = weight;

				G_edges[edge_counter] = E(vertex_counter+1, vertex_counter);
				G_weights[edge_counter++] = weight;
			}
			if(col<c-1) {  // except the last column
				int weight = rand()%weight_max + weight_min;
				G_edges[edge_counter] = E(vertex_counter, vertex_counter+r);
				G_weights[edge_counter++] = weight;

				G_edges[edge_counter] = E(vertex_counter+r, vertex_counter);
				G_weights[edge_counter++] = weight;
			}
			vertex_counter++;
		}
	}

	Graph G(G_edges, G_edges + sizeof(G_edges) / sizeof(E), G_weights, num_nodes);

	// add coordinates
	for(int col=0;col<c;col++) {
		for(int row=0;row<r;row++) {
			G[col*r+row].x = col;
			G[col*r+row].y = row;
		}
	}

	return G;
}


Graph random_graph(const int num_of_vertices, const int num_of_edges, const int weight_min, const int weight_max) {
	Graph G;
	mt19937 rng;
	rng.seed(uint32_t(time(0)));
	uniform_int<> u(0,1000);
	variate_generator<mt19937&, uniform_int<> > rng1(rng,u);
	generate_random_graph(G, num_of_vertices, num_of_edges, rng, false , false);

	// add weights
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
		int weight = rand()%weight_max + weight_min;
		put(edge_weight_t(), G, *ei, weight);
	}

	return G;
}


Graph my_reverse_graph(const Graph& G) {
	Graph reverse_G;
	copy_graph(make_reverse_graph(G), reverse_G);
	return reverse_G;
}


void random_s_t_grid(const Graph& G, Vertex& s, Vertex& t, const int r, const int c) {
	s = vertex(rand()%r, G);
	t = vertex((c-1)*r + rand()%r, G);
}


void random_s_t_random(const Graph& G, Vertex& s, Vertex& t) {
	mt19937 rng;
	rng.seed(uint32_t(time(0)));
	uniform_int<> u(0,1000);
	variate_generator<mt19937&, uniform_int<> > rng1(rng,u);

	s = random_vertex(G, rng1);
	t = random_vertex(G, rng1);
}


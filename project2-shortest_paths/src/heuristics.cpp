#include "heuristics.hpp"
#include "graphs.hpp"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/graph/random.hpp>

#include <vector>
#include <algorithm>
#include <limits>


std::vector<double> heuristic_euclidean(const Graph& G, const Vertex& t) {

	std::vector<double> h(num_vertices(G));
	int xt = G[t].x;
	int yt = G[t].y;
	int h_counter = 0;

	graph_traits<Graph>::vertex_iterator vi;
	for(vi = vertices(G).first; vi != vertices(G).second; ++vi) {
		int xi = G[*vi].x;
		int yi = G[*vi].y;
		double hi = sqrt(pow(xi-xt,2)+pow(yi-yt,2));
		h[*vi] = hi;
	}

	return h;
}


// find the new local h for the given L
std::vector<double> find_new_bounds(const Graph& G, const Vertex& L, const Vertex& t, std::vector<double>& d) {

	std::vector<double> h(num_vertices(G));

	std::vector<double> distance_map_p (num_vertices(G));
	dijkstra_shortest_paths(
		G,
		L,
		distance_map(make_iterator_property_map(distance_map_p.begin(), get(vertex_index, G)))
	);

	Graph reverse_G = my_reverse_graph(G);
	std::vector<double> distance_map_p_r (num_vertices(G));
	dijkstra_shortest_paths(
		reverse_G,
		L,
		distance_map(make_iterator_property_map(distance_map_p_r.begin(), get(vertex_index, reverse_G)))
	);


	graph_traits<Graph>::vertex_iterator vi;

	if(d.empty())
		d = distance_map_p;
	else
		for(vi = vertices(G).first; vi != vertices(G).second; ++vi)
			d[*vi] = std::min(d[*vi], distance_map_p[*vi]);


	for(vi = vertices(G).first; vi != vertices(G).second; ++vi) {
		double lower_bound1 = distance_map_p[t] - distance_map_p[*vi];
		double lower_bound2 = distance_map_p_r[*vi] - distance_map_p_r[t];
		h[*vi] = std::max(lower_bound1, lower_bound2);
	}

	return h;
}


Vertex pick_landmark(const Graph& G, const std::vector<Vertex> landmarks, const Vertex& t, std::vector<double>& d) {

	Vertex L;

	if(landmarks.empty()) { // first landmark - random pick
		mt19937 rng;
		rng.seed(uint32_t(time(0)));
		uniform_int<> u(0, num_vertices(G)-1);
		variate_generator<mt19937&, uniform_int<> > rng1(rng,u);

		L = random_vertex(G, rng1);
		return L;
	} else {
		double max_element = d[0];
		int max_index = 0, i = 0;

		// pick farest nodde for landmark
		for(std::vector<double>::iterator it = d.begin(); it != d.end(); it++, i++) {
			if(*it != std::numeric_limits<double>::max() && *it > max_element) {
				max_element = *it;
				max_index = i;
			}
		}
		return max_index;
	}
}


std::vector<double> heuristic_landmarks(const Graph& G, const Vertex& t, int num_of_landmarks) {

	std::vector<double> h(num_vertices(G)); // final answer

	std::vector<double> distances (num_vertices(G));
	std::vector<Vertex> landmarks;

	std::vector<std::vector<double>> hs (num_of_landmarks);

	for(int i=0;i<num_of_landmarks;i++) {
		Vertex L = pick_landmark(G, landmarks, t, distances);
		landmarks.push_back(L);

		hs[i] = find_new_bounds(G, L, t, distances);
	}


	graph_traits<Graph>::vertex_iterator vi;

	// construct the final h combining the locals
	for(vi = vertices(G).first; vi != vertices(G).second; ++vi) {
		double max_hi = -1;
		for(int i=0;i<num_of_landmarks;i++) {
			max_hi = std::max(hs[i][*vi], max_hi);
		}
		h[*vi] = max_hi;
	}

	return h;
}


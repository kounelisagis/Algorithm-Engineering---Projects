#include "basics.hpp"
#include "algorithms.hpp"
#include "graphs.hpp"
#include "heuristics.hpp"

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/graph/copy.hpp>

#include <limits>
#include <iostream>
#include <ctime>


// time
boost::posix_time::ptime mst1, mst2;
boost::posix_time::time_duration msdiff;


void testbench_grid(const int r, const int c, const int weight_min, const int weight_max) {

	Graph G = get_grid_graph(r, c, weight_min, weight_max);
	std::cout << std::endl << "GRID GRAPH of " << num_vertices(G) << " Nodes" << std::endl;

	// origin and end destination
	Vertex s, t; random_s_t_grid(G, s, t, r, c);
	std::cout << "s: " << s << " | t: " << t << std::endl;


	// TRUE RESULT
	std::vector<double> distance_map_p_right (num_vertices(G));
	dijkstra_shortest_paths(
		G,
		s,
		distance_map(make_iterator_property_map(distance_map_p_right.begin(), get(vertex_index, G)))
	);
	// -------------

	// A STAR
	Graph G_copy;
	copy_graph(G, G_copy);

	std::vector<double> h = heuristic_euclidean(G, t);
	std::vector<double> distance_map_A_star (num_vertices(G));

	mst1 = boost::posix_time::microsec_clock::local_time();
	int examined_vertexes = A_star(G, s, t, distance_map_A_star, h);
	mst2 = boost::posix_time::microsec_clock::local_time();
	msdiff = mst2 - mst1;

	std::cout << "\n--> A* Examined: "<< examined_vertexes;
	std::cout << " Nodes in " << msdiff.total_milliseconds() << "ms" << std::endl;
	std::cout << "Distance from s to t was found to be " << (distance_map_A_star[t] + h[s]) << std::endl;
	std::cout << "Result is: " << (std::abs(distance_map_A_star[t] + h[s] - distance_map_p_right[t]) < 1 ? "RIGHT" : "WRONG") << std::endl;


	// DIJKSTRA
	std::vector<double> distance_map_p_Dijkstra (num_vertices(G_copy));
	
	mst1 = boost::posix_time::microsec_clock::local_time();
	examined_vertexes = DijkstraSP(G_copy, s, t, distance_map_p_Dijkstra);
	mst2 = boost::posix_time::microsec_clock::local_time();
	msdiff = mst2 - mst1;

	std::cout << "\n--> DijkstraSP Examined: "<< examined_vertexes;
	std::cout << " Nodes in " << msdiff.total_milliseconds() << "ms" << std::endl;
	std::cout << "Distance from s to t was found to be " << distance_map_p_Dijkstra[t] << std::endl;
	std::cout << "Result is: " << (distance_map_p_Dijkstra[t] == distance_map_p_right[t] ? "RIGHT" : "WRONG") << std::endl;
	// -------------
	
	std::cout << "---------------" << std::endl;

}


void testbench_random(const int num_of_vertices, const int num_of_edges, const int weight_min, const int weight_max) {

	Graph G = random_graph(num_of_vertices, num_of_edges, weight_min, weight_max);
	std::cout << std::endl << "RANDOM GRAPH of " << num_vertices(G) << " Nodes" << std::endl;

	// origin and end destination
	Vertex s, t; random_s_t_random(G, s, t);
	std::cout << "s: " << s << " | t: " << t << std::endl;


	// TRUE RESULT - Verify Results
	std::vector<double> distance_map_p_right (num_vertices(G));
	dijkstra_shortest_paths(
		G,
		s,
		distance_map(make_iterator_property_map(distance_map_p_right.begin(), get(vertex_index, G)))
	);
	// -------------

	// A STAR
	for(int num_of_landmarks=2;num_of_landmarks<=4;num_of_landmarks++) {
		Graph G_copy;
		copy_graph(G, G_copy);

		std::vector<double> h = heuristic_landmarks(G_copy, t, num_of_landmarks);
		std::vector<double> distance_map_A_star (num_vertices(G_copy));
		mst1 = boost::posix_time::microsec_clock::local_time();
		int examined_vertexes = A_star(G_copy, s, t, distance_map_A_star, h);
		mst2 = boost::posix_time::microsec_clock::local_time();
		msdiff = mst2 - mst1;

		std::cout << "\n--> A* Examined (num_of_landmarks=" << num_of_landmarks << "): "<< examined_vertexes;
		std::cout << " Nodes in " << msdiff.total_milliseconds() << "ms" << std::endl;
		if(distance_map_A_star[t] == std::numeric_limits<double>::max() && distance_map_p_right[t] == std::numeric_limits<double>::max()) {
			std::cout << "Distance from s to t was found to be infinite" << std::endl;
			std::cout << "Result is: RIGHT" << std::endl;
		} else {
			std::cout << "Distance from s to t was found to be " << (distance_map_A_star[t] + h[s]) << std::endl;
			std::cout << "Result is: " << (distance_map_A_star[t] + h[s] == distance_map_p_right[t] ? "RIGHT" : "WRONG") << std::endl;
		}
	}


	// DIJKSTRA
	std::vector<double> distance_map_p_Dijkstra (num_vertices(G));
	mst1 = boost::posix_time::microsec_clock::local_time();
	int examined_vertexes = DijkstraSP(G, s, t, distance_map_p_Dijkstra);
	mst2 = boost::posix_time::microsec_clock::local_time();
	msdiff = mst2 - mst1;

	std::cout << "\n--> DijkstraSP Examined: " << examined_vertexes << " Nodes in " << msdiff.total_milliseconds() << "ms" << std::endl;
	if(distance_map_p_Dijkstra[t] == std::numeric_limits<double>::max() && distance_map_p_right[t] == std::numeric_limits<double>::max()) {
		std::cout << "Distance from s to t was found to be infinite" << std::endl;
		std::cout << "Result is: RIGHT" << std::endl;
	} else {
		std::cout << "Distance from s to t was found to be " << distance_map_p_Dijkstra[t] << std::endl;
		std::cout << "Result is: " << (distance_map_p_Dijkstra[t] == distance_map_p_right[t] ? "RIGHT" : "WRONG") << std::endl;
	}
	// -------------
	std::cout << "---------------" << std::endl;

}


int main() {

	testbench_grid(30, 1000, 1, 100);
	testbench_grid(60, 1000, 1, 100);
	testbench_grid(80, 1000, 1, 100);
	testbench_grid(30, 1000, 1, 10000);
	testbench_grid(60, 1000, 1, 10000);
	testbench_grid(80, 1000, 1, 10000);

	testbench_random(10000, 20000, 1, 100);
	testbench_random(20000, 40000, 1, 100);
	testbench_random(60000, 120000, 1, 100);
	testbench_random(10000, 20000, 1, 10000);
	testbench_random(20000, 40000, 1, 10000);
	testbench_random(60000, 120000, 1, 10000);

	return 0;
}


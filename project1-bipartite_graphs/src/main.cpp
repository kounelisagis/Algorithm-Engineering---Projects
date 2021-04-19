#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/core/queue.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/array.h"
#include "LEDA/system/timer.h"
#include "LEDA/graph/graph_misc.h"


#include "graph_generators.h"
#include "aux_functions.h"
#include "my_bipar_checker.h"

#include <iostream>
#include <string>

using namespace leda;


void testbench(ugraph& G, const std::string& message, int ITERATIONS=1000) {

	ugraph H;
	CopyGraph(H, G);
	H.make_bidirected();
	H.make_directed();
	
	float my_timer = 0;
	float leda_timer = 0;

	timer f_timer;
	bool my_resutls[ITERATIONS];
	bool leda_resutls[ITERATIONS];


	for(int iter=0;iter<ITERATIONS;iter++) {
		list<node> V1, V2;

		f_timer.start();
		my_resutls[iter] = my_bipar_checker(G, V1, V2);
		f_timer.stop();
	}
	my_timer = f_timer.elapsed_time();


	f_timer.reset();
	for(int iter=0;iter<ITERATIONS;iter++) {
		list<node> A, B;

		f_timer.start();
		leda_resutls[iter] = Is_Bipartite(H, A, B);
		f_timer.stop();
	}
	leda_timer = f_timer.elapsed_time();


	for(int iter=0;iter<ITERATIONS;iter++)
		if(my_resutls[iter]!=leda_resutls[iter]) {
			std::cout << "==============================" << std::endl;
			std::cout << message << std::endl;
			std::cout << "Results are NOT identical" << std::endl;
			std::cout << "==============================" << std::endl;

			exit(1); // Results are not equal
		}


	std::cout << "==============================" << std::endl;
	std::cout << message << std::endl;
	std::cout << "Results are identical" << std::endl;
	std::cout << "==============================" << std::endl;

	std::cout << "My solution:" << std::endl;
	std::cout << "Real time spent: " << my_timer << std::endl;
	std::cout << "Amortized time spent: " << my_timer/ITERATIONS << std::endl;
	std::cout << "------------------------------" << std::endl;

	std::cout << "LEDA's solution:" << std::endl;
	std::cout << "Real time spent: " << leda_timer << std::endl;
	std::cout << "Amortized time spent: " << leda_timer/ITERATIONS << std::endl;
	std::cout << "------------------------------" << std::endl;


	// my_print_lists(G, V1, V2);
	// my_print_lists(H, A, B);
}


int main() {

	// my_print_graph(G);
	long long int sizes1[] = {10000,40000,90000};
	for(int i=0;i<3;i++) {
		ugraph G = nested_sqrs(sizes1[i]);
		testbench(G, "Nested Squares Graph of " + std::to_string(sizes1[i]) + " nodes");
	}

	long long int sizes2[] = {10001,40001,90001};
	for(int i=0;i<3;i++) {
		ugraph G = ring_graph(sizes2[i]);
		testbench(G, "Ring Graph of " + std::to_string(sizes2[i]) + " nodes");
	}

	long long int sizes3[] = {500,1000,1500};
	for(int i=0;i<3;i++) {
		ugraph G = four_lvl_graph(sizes3[i]);
		testbench(G, "Four Levels Graph of " + std::to_string(sizes3[i]) + " nodes per level");
	}

	return 0;
}

#include "aux_functions.h"


/* This function is responsible for printing the graph in format compatible with https://dreampuf.github.io/GraphvizOnline/ */
void my_print_graph(const ugraph& G) {
	edge e;
	forall_edges(e, G) {
		std::cout << "\"";
		G.print_node(source(e));
		std::cout << "\" -- \"";
		G.print_node(target(e));
		std::cout << "\";" << std::endl;
	}
}


/* This function is responsible for printing the node lists V1 and V2 of graph G */
void my_print_lists(const ugraph& G, const list<node>& V1, const list<node>& V2) {
	node v;
	std::cout << "List1:" << std::endl;
	forall(v,V1) {
		G.print_node(v);
		std::cout << " ";
	}
	std::cout << std::endl << std::endl << "List2:" << std::endl;
	forall(v,V2) {
		G.print_node(v);
		std::cout << " ";
	}
}

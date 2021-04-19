#include "graph_generators.h"


ugraph nested_sqrs(const int n) {

	assert (n>0 && n%4==0);

	ugraph G;
	int sqrs = n/4;
	array<array<node>> sqr_nodes(sqrs);

	// create the squares
	for(int i=0;i<sqrs;i++) {
		array<node> new_sqr(4); // 0-> upLeft, 1-> upRight, 2-> downRight, 3->downLeft.

		// populate nodes
		for(int j=0;j<4;j++)
			new_sqr[j] = G.new_node();

		// populate edges
		for(int j=0;j<4;j++)
			G.new_edge(new_sqr[j], new_sqr[(j+1)%4]);

		sqr_nodes[i] = new_sqr;
	}

	// add edges between squares
	for(int i=0;i<sqrs-1;i++) {
		array<node> curr_sqr = sqr_nodes[i];
		array<node> next_sqr = sqr_nodes[i+1];
		for(int j=0;j<4;j++)
			G.new_edge(curr_sqr[j], next_sqr[j]);
	}

	return G;
}


ugraph ring_graph(const int n) {

	assert (n>2);

	ugraph G;
	node s = G.new_node();
	node prev = s;

	for(int i=0;i<n-1;i++){
		node curr = G.new_node();
		G.new_edge(prev,curr);
		prev = curr;
	}
	G.new_edge(prev,s);

	return G;
}


ugraph four_lvl_graph(const int k) {

	assert (k>0);

	const int lvls = 4;
	ugraph G;
	random_source S;

	array<node> L1(k);
	array<node> L2(k);
	array<node> L3(k);
	array<node> L4(k);

	// populate levels with nodes k nodes each
	for(int i=0;i<k;i++) {
		L1[i] = G.new_node();
		L2[i] = G.new_node();
		L3[i] = G.new_node();
		L4[i] = G.new_node();
	}

	// add basic edges between lvls
	for(int i=0;i<k;i++) {
		G.new_edge(L1[i], L2[i]);
		G.new_edge(L2[i], L3[i]);
		G.new_edge(L3[i], L4[i]);
	}

	// randomly select nodes of levels 1,2,3
	int index1 = S(0,k-1), index2 = S(0,k-1), index3 = S(0,k-1);

	// make the connections
	for(int i=0;i<k;i++) {
		if(index1!=i)
			G.new_edge(L1[index1], L2[i]);
		if(index2!=i)
			G.new_edge(L2[index2], L3[i]);
		if(index3!=i)
			G.new_edge(L3[index3], L4[i]);
	}

	// randomly select one node per level
	index1 = S(0,k-1), index2 = S(0,k-1), index3 = S(0,k-1); int index4 = S(0,k-1);

	// make the connections
	G.new_edge(L1[index1], L3[index3]);
	G.new_edge(L2[index2], L4[index4]);

	return G;
}

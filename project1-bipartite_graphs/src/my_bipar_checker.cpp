#include "my_bipar_checker.h"


list<node> my_BFS(const ugraph& G, node s, node_array<int>& dist, node_array<edge>& pred, node_array<Color>& col) {

	node v,w; edge e;

	// initialization
	forall_nodes(v,G)
		dist[v] = -1;
	dist[s] = 0;

	list<node> visited;
	visited.append(s);
	pred[s] = nil;

	queue<node> Q;
	Q.append(s);

	// source node coloring
	col[s] = green;

	while (!Q.empty()) {
		v = Q.pop();
		forall_adj_edges(e,v) {
			node w = v==source(e) ? target(e) : source(e);

			if (dist[w] < 0) {
				Q.append(w);
				visited.append(w);

				dist[w] = dist[v] + 1;
				pred[w] = e;

				// node coloring
				col[w] = dist[w]%2==0 ? green : blue;
			}
		}
	}

	return visited;
}


bool my_bipar_checker(const ugraph& G, list<node>& V1, list<node>& V2) {

	node s = G.choose_node();
	node_array<int> dist(G);
	node_array<edge> pred(G);
	node_array<Color> col(G);


	list<node> visited = my_BFS(G, s, dist, pred, col);

	node v, u; edge e;

	// check if adjacent vertices have the same colour - graph is not bipartite
	forall_edges(e,G) {
		v = source(e);
		u = target(e);
		if(col[v] == col[u]) {
			// find the paths to the lowest common ancestor
			while(v!=u) {
				V1.push_front(v);
				V1.push_back(u);

				e = pred[v];
				v = v==source(e) ? target(e) : source(e);
				e = pred[u];
				u = u==source(e) ? target(e) : source(e);
			}
			V1.push_back(s);
			return false;
		}
	}

	// if G is bipartite then fill the lists
	forall_nodes(v,G) {
		if(col[v] == green)
			V1.append(v);
		else
			V2.append(v);
	}

	return true;
}

#include "BF.hpp"

BF::BF() {
    square_counter = 1;
}

node BF::maketree(const std::string& A) {
    TreeNodeInfo * n_info = new TreeNodeInfo();

    if(A.empty()) {
        n_info->type = Square;
        n_info->label = boost::lexical_cast<std::string>(square_counter++);
    } else {
        n_info->type = Round;
        n_info->label = A;
    }

    node v = G.new_node(n_info);
    return v;
}

Types BF::type(const node& u) {
    return ((TreeNodeInfo*)G.inf(u))->type;
}

std::string BF::label(const node& u) {
    return ((TreeNodeInfo*)G.inf(u))->label;

    exit(-1);
}

node BF::parent(const node& u) {
    edge e = G.first_in_edge(u);
    if(!e)
    return NULL;

    node parent = G.source(e);
    return parent;
}


void BF::evert(const node& x) {
    // iterate incoming edges
    // recursively change all of them to outgoing
    edge e = G.first_in_edge(x);
    if(e) {
        node s = G.source(e);
        evert(s);
        G.rev_edge(e);
    }
}

void BF::link(node x, node y) {
    G.new_edge(y,x,1);
}

node BF::getRoot(const node &v) {
    node p = parent(v);
    if(p)
    return getRoot(p);

    return v;
}

int BF::num_of_childs(node x) {
    node_array<int> dist1(G,-1);
    list<node> L=BFS(G,x,dist1);

    int counter = 0;
    node u;
    forall(u, L) {
        if(type(u)==Square)
            counter++;
    }

    return counter;
}

std::vector<node> BF::findpath(node v, node u) {

    assert (type(u)==Square && type(v)==Square);

    if (v == u) {
        return std::vector<node> {parent(u)};
    }

    v = parent(v);
    u = parent(u);

    std::vector<node> path_v, path_u;

    while (v) {
        path_v.push_back(v);
        v = parent(v);
    }
    while (u) {
        path_u.push_back(u);
        u = parent(u);
    }

    std::vector<node> path;
    std::vector<node>::reverse_iterator it_v, it_u;
    node lca;

    for(it_v = path_v.rbegin(), it_u = path_u.rbegin(); it_v != path_v.rend() && it_u != path_u.rend(); it_v++, it_u++ ) {
        if (*it_v == *it_u) {
            lca = *it_v;
        } else {
            path.push_back(*it_v);
            path.push_back(*it_u);
        }
    }

    while(it_v != path_v.rend()) {
        path.push_back(*it_v);
        it_v++;
    }

    while(it_u != path_u.rend()) {
        path.push_back(*it_u);
        it_u++;
    }

    path.push_back(lca);
    return path;
}


void BF::condensepath(std::vector<node> path, const std::string& A) {

    // all path contains round nodes

    node w = path.back();
    path.pop_back();

    node w_parent = parent(w);
    node r_hat;
    if (w_parent == NULL)
        r_hat = w;
    else
        r_hat = maketree(A);


    if(type(w) == Round && w_parent != NULL) {
        link(r_hat, w_parent);
    } else if(type(w) == Square) {
        link(r_hat, w);
    }

    edge e;

    for(std::vector<node>::iterator it = path.begin(); it != path.end(); ++it) {

        forall_adj_edges(e, *it) {
            node target = G.target(e);
            G.del_edge(e);
            G.new_edge(r_hat, target);
        }

        e = G.first_in_edge(*it);
        G.del_edge(e);

        if (type(*it)==Round)
            G.del_node(*it);
        else
            G.new_edge(r_hat, *it);
    }
}


/* This function is responsible for printing the graph in format compatible with https://dreampuf.github.io/GraphvizOnline/ */
void BF::print_graph() {
  node u; edge e;

  int counter = 0;

  std::cout << "digraph G {" << std::endl;

  forall_nodes(u, G) {
      std::cout << label(u) << " [label=" << label(u) << ", shape=" << (type(u)==Square?"box":"circle") << "];" << std::endl;
  }

  forall_edges(e, G) {
      std::cout << label(source(e));
      std::cout << "->";
      std::cout << label(target(e));
      std::cout << ";" << std::endl;
  }
  std::cout << "}" << std::endl;

}

#include "BBF.hpp"

BBF::BBF() {
    square_counter = 1;
}

node BBF::maketree(const std::string& A) {
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

Types BBF::type(const node& u) {
    return ((TreeNodeInfo*)G.inf(u))->type;
}

std::string BBF::label(const node& u) {
    // if(type(u) == Round)
    return ((TreeNodeInfo*)G.inf(u))->label;

    exit(-1);
}

node BBF::parent(const node& u) {
    edge e = G.first_in_edge(u);
    if(!e)
    return NULL;

    node parent = G.source(e);
    return parent;
}

void BBF::evert(const node& x) {
    // iterate incoming edges
    // recursively change all of them to outgoing
    edge e = G.first_in_edge(x);
    if(e) {
        node s = G.source(e);
        evert(s);
        G.rev_edge(e);
    }
}

void BBF::link(node x, node y) {
    G.new_edge(y,x,1);
}

node BBF::getRoot(const node &v) {
    node p = parent(v);
    if(p)
    return getRoot(p);

    return v;
}

int BBF::num_of_childs(node x) {
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

std::vector<node> BBF::findpath(node v, node u) {

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


void BBF::condensepath(std::vector<node> path, const std::string& A) {

    // all path contains round nodes
    node new_node = path.back();
    path.pop_back();

    ((TreeNodeInfo*)G.inf(new_node))->label = A;

    for(std::vector<node>::iterator it = path.begin(); it != path.end(); ++it) {

        edge e;

        forall_adj_edges(e, *it) {
            node target = G.target(e);
            G.del_edge(e);
            G.new_edge(new_node, target);
        }

        e = G.first_in_edge(*it);
        G.del_edge(e);

        G.del_node(*it);
    }
}


/* This function is responsible for printing the graph in format compatible with https://dreampuf.github.io/GraphvizOnline/ */
void BBF::print_graph() {
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

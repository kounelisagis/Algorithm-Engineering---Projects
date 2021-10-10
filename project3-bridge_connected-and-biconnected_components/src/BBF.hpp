#ifndef BBF_H
#define BBF_H

#include "LEDA/graph/graph.h"
#include <boost/lexical_cast.hpp> // int to string
#include <LEDA/graph/basic_graph_alg.h>

#include <iostream>
#include <string>
#include <vector>

using namespace leda;


#include "basics.hpp"


class BBF {
  public:
    GRAPH<TreeNodeInfo*,int> G;
    int square_counter;

    BBF();
    node maketree(const std::string&);
    Types type(const node&);
    std::string label(const node&);
    node parent(const node&);
    std::string find_block(const node&);
    void evert(const node&);
    void link(node, node);
    node getRoot(const node&);
    int num_of_childs(node);
    std::vector<node> findpath(node, node);
    void condensepath(std::vector<node>, const std::string&);
    /* This function is responsible for printing the graph in format compatible with https://dreampuf.github.io/GraphvizOnline/ */
    void print_graph();
};


#endif

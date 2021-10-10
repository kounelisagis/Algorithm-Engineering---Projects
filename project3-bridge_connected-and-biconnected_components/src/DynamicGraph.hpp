#ifndef DYNAMICGRAPH_H
#define DYNAMICGRAPH_H

#include "LEDA/graph/ugraph.h"

#include <iostream>
#include <string>

#include "BF.hpp"
#include "BBF.hpp"
#include "basics.hpp"

using namespace leda;

typedef struct{
  node BF_node;
  node BBF_node;
}GraphNodeInfo;



class DynamicGraph {
  public:

    UGRAPH<GraphNodeInfo*, int> G;
    BF G_BF;
    BBF G_BBF;

    DynamicGraph();
    node make_vertex(const std::string&);

    node BF_node(const node&);
    node BBF_node(const node&);

    std::string find_block(const node&, const node&);
    std::string find_bridge_block(const node&);

    void insert_edge(const node&, const node&, const std::string&);
    void insert_edge_BBF(const node&, const node&, const std::string&);
    void insert_edge_BF(const node&, const node&, const std::string&);
};



#endif

#include "DynamicGraph.hpp"

DynamicGraph::DynamicGraph() {}

node DynamicGraph::make_vertex(const std::string& A) {
    node BF_node = G_BF.maketree(std::string());

    node BBF_node = G_BBF.maketree(std::string());
    G_BBF.link(BBF_node, G_BBF.maketree(A));

    GraphNodeInfo * node_info = new GraphNodeInfo();
    node_info->BF_node = BF_node;
    node_info->BBF_node = BBF_node;

    node v = G.new_node(node_info);

    return v;
}

node DynamicGraph::BF_node(const node& u) {
    return G.inf(u)->BF_node;
}

node DynamicGraph::BBF_node(const node& u) {
    return G.inf(u)->BBF_node;
}

std::string DynamicGraph::find_block(const node& u, const node& v) {
    node u_tree = BF_node(u);
    node v_tree = BF_node(v);

    std::vector<node> path = G_BF.findpath(v_tree, u_tree);

    for(std::vector<node>::iterator it = path.begin(); it != path.end(); ++it) {
        if(G_BF.type(*it) == Round) {
            return G_BF.label(*it);
        }
    }

}

std::string DynamicGraph::find_bridge_block(const node& u) {
    node u_tree = BBF_node(u);

    std::vector<node> path = G_BBF.findpath(u_tree, u_tree);
    return G_BBF.label(path[0]);
}

void DynamicGraph::insert_edge(const node& u, const node& v, const std::string& A) {
    edge e = G.new_edge(u, v);

    insert_edge_BBF(u, v, A);
    insert_edge_BF(u, v, A);
}

void DynamicGraph::insert_edge_BBF(const node& u, const node& v, const std::string& A) {

    node u_tree = BBF_node(u);
    node v_tree = BBF_node(v);

    node u_tree_parent = G_BBF.parent(u_tree);
    node v_tree_parent = G_BBF.parent(v_tree);

    std::string label_u = G_BBF.label(u_tree_parent);
    std::string label_v = G_BBF.label(v_tree_parent);

    if (label_v != label_u) {
    // find the root of the component for each
    // If it's the same, then the component is the same

    node root1 = G_BBF.getRoot(u_tree);
    node root2 = G_BBF.getRoot(v_tree);

        if(root1 != root2) { // different components
            if ( G_BBF.num_of_childs(u_tree_parent) <= G_BBF.num_of_childs(v_tree_parent) ) {
                G_BBF.evert(u_tree_parent);
                G_BBF.link(u_tree_parent, v_tree_parent);
            } else {
                G_BBF.evert(v_tree_parent);
                G_BBF.link(v_tree_parent, u_tree_parent);
            }
        } else { // same component
            G_BBF.condensepath(G_BBF.findpath(u_tree, v_tree), A);
        }
    }
}


void DynamicGraph::insert_edge_BF(const node& u, const node& v, const std::string& A) {

    node u_tree = BF_node(u);
    node v_tree = BF_node(v);

    node root1 = G_BF.getRoot(u_tree);
    node root2 = G_BF.getRoot(v_tree);

    if(root1 != root2) { // different components

        node r_hat = G_BF.maketree(A);

        if (G_BF.parent(u_tree) == NULL && G_BF.parent(v_tree) == NULL) {
            G_BF.link(u_tree, r_hat);
            G_BF.link(v_tree, r_hat);
        } else if ( G_BF.parent(v_tree) != NULL && G_BF.num_of_childs(u_tree) <= G_BF.num_of_childs(v_tree) ) {
            G_BF.evert(u_tree);
            G_BF.link(u_tree, r_hat);
            G_BF.link(r_hat, v_tree);
        } else {
            G_BF.evert(v_tree);
            G_BF.link(v_tree, r_hat);
            G_BF.link(r_hat, u_tree);
        }
    } else { // same component
        G_BF.condensepath(G_BF.findpath(u_tree, v_tree), A);
    }
}

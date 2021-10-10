#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/core/list.h"
#include "LEDA/core/random_source.h"
#include "LEDA/system/timer.h"
#include <boost/lexical_cast.hpp> // int to string

#include <iostream>
#include <string>
#include <vector>

#include "DynamicGraph.hpp"

using namespace leda;


std::string give_me_the_next_letter() {
  static int n = 0;
  char c = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[(n++)%52];

  return std::string(1, c);
}


void testbench(int initial_nodes, int initial_edges, int extra_nodes, int extra_edges, int num_find_bbf, int num_find_bf) {

  leda::random_source S;

  DynamicGraph my_G;
  std::vector<node> vertices;

  float nodes_time = 0, edges_time = 0;
  timer f_timer;


  for(int i=0;i<initial_nodes;i++) {
    f_timer.start();
    node v = my_G.make_vertex(std::string(give_me_the_next_letter()));
    f_timer.stop();

    vertices.push_back(v);
  }

  nodes_time = f_timer.elapsed_time();
  f_timer.reset();

  int edges_created = 0;
  int v1, v2;

  while (edges_created < initial_edges) {
    S >> v1; S >> v2;
    v1 = v1%initial_nodes; v2 = v2%initial_nodes;
    if(v1!=v2) {
      f_timer.start();
      my_G.insert_edge(vertices[v1], vertices[v2], give_me_the_next_letter());
      f_timer.stop();

      edges_created++;
    }
  }

  edges_time = f_timer.elapsed_time();
  f_timer.reset();


  std::cout << "========INITIALIZATION========" << std::endl;
  std::cout << "Time spent for Nodes: " << nodes_time << std::endl;
  std::cout << "Amortized time spent for Nodes: " << nodes_time/initial_nodes << std::endl;
  std::cout << "Time spent for Edges: " << edges_time << std::endl;
  std::cout << "Amortized time spent for Edges: " << edges_time/initial_edges << std::endl;
  std::cout << "==============================" << std::endl;



  for(int i=0;i<extra_nodes;i++) {
    f_timer.start();
    node v = my_G.make_vertex(std::string(give_me_the_next_letter()));
    f_timer.stop();

    vertices.push_back(v);
  }

  nodes_time = f_timer.elapsed_time();
  f_timer.reset();


  edges_created = 0;
  v1, v2;

  while (edges_created < extra_edges) {
    S >> v1; S >> v2;
    v1 = v1%(initial_nodes+extra_nodes); v2 = v2%(initial_nodes+extra_nodes);
    if(v1!=v2) {
      f_timer.start();
      my_G.insert_edge(vertices[v1], vertices[v2], give_me_the_next_letter());
      f_timer.stop();

      edges_created++;
    }
  }

  edges_time = f_timer.elapsed_time();
  f_timer.reset();


  std::cout << "========ONLINE=UPDATES========" << std::endl;
  std::cout << "Time spent for Nodes: " << nodes_time << std::endl;
  std::cout << "Amortized time spent for Nodes: " << nodes_time/initial_nodes << std::endl;
  std::cout << "Time spent for Edges: " << edges_time << std::endl;
  std::cout << "Amortized time spent for Edges: " << edges_time/initial_edges << std::endl;
  std::cout << "==============================" << std::endl;

  float bbf_time = 0, bf_time = 0;

  int bbf_executed = 0, bf_executed = 0;
  while (bbf_executed < num_find_bbf) {
    S >> v1;
    v1 = v1%(initial_nodes+extra_nodes);
    f_timer.start();
    my_G.find_bridge_block(vertices[v1]);
    f_timer.stop();

    bbf_executed++;
  }

  bbf_time = f_timer.elapsed_time();
  f_timer.reset();

  while (bf_executed < num_find_bf) {
    edge e = my_G.G.choose_edge(); node s = my_G.G.source(e), t = my_G.G.target(e);
    f_timer.start();
    my_G.find_block(s, t);
    f_timer.stop();

    bf_executed++;
  }

  bf_time = f_timer.elapsed_time();


  std::cout << "==========FIND=BLOCK==========" << std::endl;
  std::cout << "Time spent for BBF: " << bbf_time << std::endl;
  std::cout << "Amortized time spent for Nodes: " << bbf_time/num_find_bbf << std::endl;
  std::cout << "Time spent for BF: " << bf_time << std::endl;
  std::cout << "Amortized time spent for Edges: " << bf_time/num_find_bf << std::endl;
  std::cout << "==============================" << std::endl;

}


void exampleGraph() {

  int N = 13;

  DynamicGraph my_G;
  std::vector<node> vertices;
  for(int i=0;i<N;i++) {
    vertices.push_back(my_G.make_vertex(std::string(give_me_the_next_letter())));
  }

  my_G.insert_edge(vertices[0], vertices[1], give_me_the_next_letter());
  my_G.insert_edge(vertices[1], vertices[2], give_me_the_next_letter());
  my_G.insert_edge(vertices[1], vertices[3], give_me_the_next_letter());
  my_G.insert_edge(vertices[2], vertices[3], give_me_the_next_letter());
  my_G.insert_edge(vertices[3], vertices[4], give_me_the_next_letter());
  my_G.insert_edge(vertices[4], vertices[5], give_me_the_next_letter());
  my_G.insert_edge(vertices[4], vertices[6], give_me_the_next_letter());
  my_G.insert_edge(vertices[5], vertices[6], give_me_the_next_letter());
  my_G.insert_edge(vertices[6], vertices[7], give_me_the_next_letter());
  my_G.insert_edge(vertices[6], vertices[8], give_me_the_next_letter());
  my_G.insert_edge(vertices[7], vertices[8], give_me_the_next_letter());
  my_G.insert_edge(vertices[6], vertices[9], give_me_the_next_letter());
  my_G.insert_edge(vertices[10], vertices[11], give_me_the_next_letter());
  my_G.insert_edge(vertices[10], vertices[12], give_me_the_next_letter());
  my_G.insert_edge(vertices[11], vertices[12], give_me_the_next_letter());



  while(1) {
    std::cout << "\n================================\n1. Print BBF\n2. Print BF\n3. Find Block\n4. Find Bridge Block\n5. Insert Node\n6. Insert Edge\n================================\n";
    int choice;
    std::cin >> choice;
    system("clear");
    switch(choice) {
      case 1:
        my_G.G_BBF.print_graph();
        break;
      case 2:
        my_G.G_BF.print_graph();
        break;
      case 3:
        int ver1, ver2;
        std::cout << "\nType v and u separated by a space: ";
        std::cin >> ver1 >> ver2;
        std:: cout << "My Block is " << my_G.find_block(vertices[ver1-1], vertices[ver2-1]) << std::endl;
        break;
      case 4:
        int ver;
        std::cout << "\nType v: ";
        std::cin >> ver;
        std:: cout << "My Bridge Block is " << my_G.find_bridge_block(vertices[ver-1]) << std::endl;
        break;
      case 5:
        vertices.push_back(my_G.make_vertex(std::string(give_me_the_next_letter())));
        std::cout << "\nNode " << ++N << " added\n";
        break;
      case 6:
        int v, u;
        std::cout << "\nType v and u of (v,u) separated by a space: ";
        std::cin >> ver1 >> ver2;
        my_G.insert_edge(vertices[ver1-1], vertices[ver2-1], give_me_the_next_letter());
        break;
      default:
        return;
    }
  }
}

int main() {

  exampleGraph();

  // int initial_nodes = 1000, initial_edges = 10000, extra_nodes = 100000, extra_edges = 10000, num_find_bbf = 80000, num_find_bf = 10000;
  // testbench(initial_nodes, initial_edges, extra_nodes, extra_edges, num_find_bbf, num_find_bf);

  return 0;
}

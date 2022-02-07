#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

  public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int l, p;
  std::cin >> l >> p;

  graph G(l);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  long total_demand = 0;

  for (int i = 0; i < l; i++) {
    int g, d;
    std::cin >> g >> d;

    total_demand += d;
    adder.add_edge(v_source, i, g);
    adder.add_edge(i, v_sink, d);
  }

  for (int i = 0; i < p; i++) {
    int f, t, c, C;
    std::cin >> f >> t >> c >> C;

    total_demand += c;
    adder.add_edge(v_source, t, c);
    adder.add_edge(f, v_sink, c);
    adder.add_edge(f, t, C-c);
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << (flow == total_demand ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}
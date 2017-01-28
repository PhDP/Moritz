#ifndef WAGNER_NETWORK_H_
#define WAGNER_NETWORK_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <set>
#include <random>

namespace wagner {

template<typename T>
class network {
  std::map<T, std::set<T>> m_net;
  void m_test_connectivity(std::map<T, bool> &vs, const T &t);

 public:
  /** Basic constructor. */
  network();

  /** Basic destructor. */
  ~network();

  /** Number of edges in the entire network. */
  auto size() const -> size_t;

  /** Number of edges in node t. */
  auto size(const T &t) const -> size_t;

  /** Number or vertices. */
  auto order() const -> size_t;

  /** Destroy all vertices and create a random geometric graph. */
  auto rgg(size_t order, double radius, std::mt19937_64 &rng) -> void;

  /** Return true if the network is strongly connected. */
  auto connected() -> bool;

  /** Return one vertex of the graph at random. */
  auto random_vertex(std::mt19937_64 &rng) -> T;

  /** Return true if the vertex is present. */
  auto has_vertex(const T &t) const -> bool;

  /** Return true if an edge is present between v0 and v1. */
  auto has_edge(const T &t0, const T &t1) -> bool;

  /** Add a vertex. */
  auto add_vertex(const T &t) -> bool;

  /** Add an edge between vertices v0 and v1. */
  auto add_edge(const T &t0, const T &t1) -> size_t;

  /** Add an edge between vertives v0 and v1 and betwen v1 and v0. Return the
   * number of edges added. */
  auto add_edges(const T &t0, const T &t1) -> size_t;

  /** Return the set of neighbors for vertex 'v'. */
  auto neighbors(const T &t) -> std::set<T>;

  /** Return the set of neighbors for vertex 'v'. */
  auto operator[](const T &t) -> std::set<T>;

  // Iterate the network:
  auto begin() -> typename std::map<T, std::set<T>>::iterator;
  auto end() -> typename std::map<T, std::set<T>>::iterator;
  auto begin() const -> typename std::map<T, std::set<T>>::const_iterator;
  auto end() const -> typename std::map<T, std::set<T>>::const_iterator;
};

template<typename T>
network<T>::network() {
  //
}

template<typename T>
network<T>::~network() {
  //
}

template<typename T>
auto network<T>::size() const -> size_t {
  size_t sum = 0;
  for (auto i : m_net) {
    sum += i.second.size();
  }
  return sum;
}

template<typename T>
auto network<T>::size(const T &t) const -> size_t {
  return m_net[t].size();
}

template<typename T>
auto network<T>::order() const -> size_t {
  return m_net.size();
}

template <typename T>
auto network<T>::m_test_connectivity(std::map<T, bool> &vs, const T &v) -> void {
  std::set<T> ns = neighbors(v);
  for (auto i : ns) {
    if (vs[i] == false) {
      vs[i] = true;
      m_test_connectivity(vs, i);
    }
  }
}

template<typename T>
auto network<T>::connected() -> bool {
  for (auto i : m_net) {
    std::map<T, bool> is;
    for (auto j : m_net) {
      is[j.first] = (j.first == i.first);
    }
    m_test_connectivity(is, i.first);

    for (auto j : is) {
      if (j.second == false) {
        return false;
      }
    }
  }
  return true;
}

template<typename T>
auto network<T>::random_vertex(std::mt19937_64 &rng) -> T {
  std::uniform_int_distribution<> unif(0, m_net.size());
  const size_t v = unif(rng);
  size_t index = 0;
  for (auto i : m_net) {
    if (v == index++) {
      return i.first;
    }
  }
  exit(EXIT_FAILURE);
}

template <typename T>
auto network<T>::rgg(size_t order, double radius, std::mt19937_64 &rng) -> void {
  std::uniform_real_distribution<> unif;
  m_net.clear();
  for (int i = 0; i < order; ++i) {
    T p(unif(rng), unif(rng));
    std::set<T> emps;
    m_net.insert(std::pair<T, std::set<T>>(p, emps));
  }
  for (auto i : m_net) {
    for (auto j : m_net) {
      if (euclidean_distance(i.first, j.first) < radius) {
        add_edges(i.first, j.first);
      }
    }
  }
}

template<typename T>
auto network<T>::has_vertex(const T &t) const -> bool {
  return m_net.find(t) != m_net.end();
}

template<typename T>
auto network<T>::has_edge(const T &t0, const T &t1) -> bool {
  if (has_vertex(t0) && has_vertex(t1)) {
    return (m_net[t0].find(t1) != m_net[t0].end());
  } else {
    return false;
  }
}

template<typename T>
auto network<T>::add_vertex(const T &t) -> bool {
  if (has_vertex(t)) {
    return false;
  } else {
    std::set<T> emps;
    m_net.insert(std::pair<T, std::set<T>>(t, emps));
    return true;
  }
}

template <typename T>
auto network<T>::add_edge(const T &t0, const T &t1) -> size_t {
  if (has_vertex(t0)) {
    if (has_edge(t0, t1)) {
      return 0;
    } else {
      m_net[t0].insert(t1);
      return 1;
    }
  } else {
    return 0;
  }
}

template<typename T>
auto network<T>::add_edges(const T &t0, const T &t1) -> size_t {
  return add_edge(t0, t1) + add_edge(t1, t0);
}

template<typename T>
auto network<T>::neighbors(const T &t) -> std::set<T> {
  return m_net[t];
}

template<typename T>
auto network<T>::operator[](const T &t) -> std::set<T> {
  return m_net[t];
}

template<typename T>
auto network<T>::begin() -> typename std::map<T, std::set<T>>::iterator {
  return m_net.begin();
}

template<typename T>
auto network<T>::end() -> typename std::map<T, std::set<T>>::iterator {
  return m_net.end();
}

template<typename T>
auto network<T>::begin() const -> typename std::map<T, std::set<T>>::const_iterator {
  return m_net.begin();
}

template<typename T>
auto network<T>::end() const -> typename std::map<T, std::set<T>>::const_iterator{
  return m_net.end();
}

template<typename T>
auto operator<<(std::ostream &os, const network<T> &net) -> std::ostream& {
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  os << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"";
  os << " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
  os << " xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";
  os << "  <graph id=\"network\" edgedefault=\"directed\">\n";

  for (auto v : net) {
    os << "    <node id=\"" << v.first << "\"/>\n";
  }
  for (auto v : net) {
    for (auto e : v.second) {
      os << "    <edge source=\"" << v.first << "\" target=\"" << e << "\"/>\n";
    }
  }
  os << "  </graph>\n</graphml>\n";
  return os;
}

}

#endif /* WAGNER_NETWORK_H_ */

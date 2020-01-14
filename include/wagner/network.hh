#ifndef WAGNER_NETWORK_H_
#define WAGNER_NETWORK_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <random>
#include "wagner/common.hh"

namespace wagner {

template<typename T>
class network {
  map<T, set<T>> m_net;
  auto m_test_connectivity(map<T, bool> &vs, const T &t) noexcept -> void;

 public:
 
  using const_iterator = typename map<T, set<T>>::const_iterator; 
 
  /** Basic constructor. */
  network() noexcept {
    //
  }

  /** Number of edges in the entire network. */
  auto size() const noexcept -> size_t {
    size_t sum = 0;
    for (auto i : m_net) sum += i.second.size();
    return sum;
  }

  /** Number of edges in node t. */
  auto size(const T &t) const noexcept -> size_t {
    return m_net[t].size();
  }

  /** Number or vertices. */
  auto order() const noexcept -> size_t {
    return m_net.size();
  }

  /** Destroy all vertices and create a random geometric graph. */
  auto rgg(size_t order, double radius, std::mt19937_64 &rng) noexcept -> void;

  // static auto make_rgg(size_t order, double radius, std::mt19937_64& rng) -> network<T>;
  // static auto make_rg(size_t order, double alpha, std::mt19937_64& rng) -> network<T>;
  // static auto make_grid(size_t rows, size_t columns) -> network<T>;

  /** Returns true if the network is strongly connected. */
  auto connected() noexcept -> bool;

  /** Returns one vertex of the graph at random. */
  auto random_vertex(std::mt19937_64 &rng) const noexcept -> const_iterator;

  /** Returns true if the vertex is present. */
  auto has_vertex(const T &t) const noexcept -> bool;

  /** Returns true if an edge is present between v0 and v1. */
  auto has_edge(const T &t0, const T &t1) const noexcept -> bool;

  /** Adds a vertex. */
  auto add_vertex(const T &t) noexcept -> bool;

  /** Adds an edge between vertices v0 and v1. */
  auto add_edge(const T &t0, const T &t1) noexcept -> size_t;

  /** Adds an edge between vertives v0 and v1 and betwen v1 and v0. Return the
   * number of edges added. */
  auto add_edges(const T &t0, const T &t1) noexcept -> size_t;

  /** Returns the set of neighbors for vertex 'v'. */
  auto neighbors(const T &t) const noexcept -> set<T> const&;

  /** Returns the set of neighbors for vertex 'v'. */
  auto operator[](const T &t) const noexcept -> set<T> const&;

  // Iterate the network:
  auto begin() noexcept -> typename map<T, set<T>>::iterator {
    return m_net.begin();
  }

  auto end() noexcept -> typename map<T, set<T>>::iterator {
    return m_net.end();
  }

  auto begin() const noexcept -> typename map<T, set<T>>::const_iterator {
    return m_net.begin();
  }

  auto end() const noexcept -> typename map<T, set<T>>::const_iterator {
    return m_net.end();
  }
};

template<typename T>
auto network<T>::m_test_connectivity(map<T, bool> &vs, const T &v) noexcept -> void {
  set<T> ns = neighbors(v);
  for (auto i : ns) {
    if (vs[i] == false) {
      vs[i] = true;
      m_test_connectivity(vs, i);
    }
  }
}

template<typename T>
auto network<T>::connected() noexcept -> bool {
  for (auto i : m_net) {
    map<T, bool> is;
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
auto network<T>::random_vertex(std::mt19937_64 &rng) const noexcept -> const_iterator {
  if (m_net.size() < 2) {
    return m_net.begin();
  }
  auto unif = std::uniform_int_distribution<size_t>(0, m_net.size() - 1);
  auto it = m_net.begin();
  std::advance(it, unif(rng));
  return it;
}

template<typename T>
auto network<T>::rgg(size_t order, double radius, std::mt19937_64 &rng) noexcept -> void {
  std::uniform_real_distribution<> unif;
  m_net.clear();
  for (int i = 0; i < order; ++i) {
    T p(unif(rng), unif(rng));
    auto emps = set<T>{};
    m_net.insert(std::pair<T, set<T>>(p, emps));
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
auto network<T>::has_vertex(const T &t) const noexcept -> bool {
  return m_net.find(t) != m_net.end();
}

template<typename T>
auto network<T>::has_edge(const T &t0, const T &t1) const noexcept -> bool {
  return has_vertex(t0) && has_vertex(t1) && (m_net.at(t0).find(t1) != m_net.at(t0).end());
}

template<typename T>
auto network<T>::add_vertex(const T &t) noexcept -> bool {
  if (has_vertex(t)) {
    return false;
  } else {
    auto emps = set<T>{};
    m_net.insert(std::pair<T, set<T>>(t, emps));
    return true;
  }
}

template<typename T>
auto network<T>::add_edge(const T &t0, const T &t1) noexcept -> size_t {
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
auto network<T>::add_edges(const T &t0, const T &t1) noexcept -> size_t {
  return add_edge(t0, t1) + add_edge(t1, t0);
}

template<typename T>
auto network<T>::neighbors(const T &t) const noexcept -> set<T> const& {
  return m_net.at(t);
}

template<typename T>
auto network<T>::operator[](const T &t) const noexcept -> set<T> const& {
  return m_net.at(t);
}

template<typename T>
auto operator<<(std::ostream &os, const network<T> &net) noexcept -> std::ostream& {
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

#endif

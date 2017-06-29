#ifndef WAGNER_NETWORK_H_
#define WAGNER_NETWORK_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <random>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>

namespace wagner {

template<typename T>
class network {
  boost::container::flat_map<T, boost::container::flat_set<T>> m_net;
  auto m_test_connectivity(boost::container::flat_map<T, bool> &vs, const T &t) noexcept -> void;

 public:
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

  /** Return true if the network is strongly connected. */
  auto connected() noexcept -> bool;

  /** Return one vertex of the graph at random. */
  auto random_vertex(std::mt19937_64 &rng) noexcept -> T; // What????

  /** Return true if the vertex is present. */
  auto has_vertex(const T &t) const noexcept -> bool;

  /** Return true if an edge is present between v0 and v1. */
  auto has_edge(const T &t0, const T &t1) noexcept -> bool;

  /** Add a vertex. */
  auto add_vertex(const T &t) noexcept -> bool;

  /** Add an edge between vertices v0 and v1. */
  auto add_edge(const T &t0, const T &t1) noexcept -> size_t;

  /** Add an edge between vertives v0 and v1 and betwen v1 and v0. Return the
   * number of edges added. */
  auto add_edges(const T &t0, const T &t1) noexcept -> size_t;

  /** Return the set of neighbors for vertex 'v'. */
  auto neighbors(const T &t) noexcept -> boost::container::flat_set<T>&;

  /** Return the set of neighbors for vertex 'v'. */
  auto operator[](const T &t) noexcept -> boost::container::flat_set<T>&;

  // Iterate the network:
  auto begin() noexcept -> typename boost::container::flat_map<T, boost::container::flat_set<T>>::iterator {
    return m_net.begin();
  }

  auto end() noexcept -> typename boost::container::flat_map<T, boost::container::flat_set<T>>::iterator {
    return m_net.end();
  }

  auto begin() const noexcept -> typename boost::container::flat_map<T, boost::container::flat_set<T>>::const_iterator {
    return m_net.begin();
  }

  auto end() const noexcept -> typename boost::container::flat_map<T, boost::container::flat_set<T>>::const_iterator {
    return m_net.end();
  }
};

template<typename T>
auto network<T>::m_test_connectivity(boost::container::flat_map<T, bool> &vs, const T &v) noexcept -> void {
  boost::container::flat_set<T> ns = neighbors(v);
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
    boost::container::flat_map<T, bool> is;
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
auto network<T>::random_vertex(std::mt19937_64 &rng) noexcept -> T {
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

template<typename T>
auto network<T>::rgg(size_t order, double radius, std::mt19937_64 &rng) noexcept -> void {
  std::uniform_real_distribution<> unif;
  m_net.clear();
  for (int i = 0; i < order; ++i) {
    T p(unif(rng), unif(rng));
    boost::container::flat_set<T> emps;
    m_net.insert(std::pair<T, boost::container::flat_set<T>>(p, emps));
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
auto network<T>::has_edge(const T &t0, const T &t1) noexcept -> bool {
  if (has_vertex(t0) && has_vertex(t1)) {
    return (m_net[t0].find(t1) != m_net[t0].end());
  } else {
    return false;
  }
}

template<typename T>
auto network<T>::add_vertex(const T &t) noexcept -> bool {
  if (has_vertex(t)) {
    return false;
  } else {
    boost::container::flat_set<T> emps;
    m_net.insert(std::pair<T, boost::container::flat_set<T>>(t, emps));
    return true;
  }
}

template <typename T>
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
auto network<T>::neighbors(const T &t) noexcept -> boost::container::flat_set<T>& {
  return m_net[t];
}

template<typename T>
auto network<T>::operator[](const T &t) noexcept -> boost::container::flat_set<T>& {
  return m_net[t];
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

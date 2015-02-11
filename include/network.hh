#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <set>
#include <random>

template <typename T> class network {
private:
  // The actual network:
  std::map<T, std::set<T>> m_net;

  // Private function to test connectivity.
  void m_test_connectivity(std::map<T, bool> &vs, const T &t);

public:
  /** Basic constructor. */
  network();

  /** Basic destructor. */
  ~network();

  /** Number of edges in the entire network. */
  unsigned int size() const;

  /** Number of edges in node t. */
  unsigned int size(const T &t) const;

  /** Number or vertices. */
  unsigned int order() const;

  /** Destroy all vertices and create a random geometric graph. */
  void rgg(unsigned int order, double radius, std::mt19937_64 &rng);

  /** Return true if the network is strongly connected. */
  bool connected();

  /** Return one vertex of the graph at random. */
  T random_vertex(std::mt19937_64 &rng);

  /** Return true if the vertex is present. */
  bool has_vertex(const T &t) const;

  /** Return true if an edge is present between v0 and v1. */
  bool has_edge(const T &t0, const T &t1);

  /** Add a vertex. */
  bool add_vertex(const T &t);

  /** Add an edge between vertices v0 and v1. */
  unsigned int add_edge(const T &t0, const T &t1);

  /** Add an edge between vertives v0 and v1 and betwen v1 and v0. Return the
   * number of edges added. */
  unsigned int add_edges(const T &t0, const T &t1);

  /** Return the set of neighbors for vertex 'v'. */
  std::set<T> neighbors(const T &t);

  /** Return the set of neighbors for vertex 'v'. */
  std::set<T> operator[](const T &t);

  // Iterate the network:
  typename std::map<T, std::set<T>>::iterator begin();
  typename std::map<T, std::set<T>>::iterator end();
  typename std::map<T, std::set<T>>::const_iterator begin() const;
  typename std::map<T, std::set<T>>::const_iterator end() const;
};

template <typename T> network<T>::network() {
  //
}

template <typename T> network<T>::~network() {
  //
}

template <typename T> unsigned int network<T>::size() const {
  unsigned int sum = 0;
  for (auto i : m_net) {
    sum += i.second.size();
  }
  return sum;
}

template <typename T> unsigned int network<T>::size(const T &t) const {
  return m_net[t].size();
}

template <typename T> unsigned int network<T>::order() const {
  return m_net.size();
}

template <typename T>
void network<T>::m_test_connectivity(std::map<T, bool> &vs, const T &v) {
  std::set<T> ns = neighbors(v);
  for (auto i : ns) {
    if (vs[i] == false) {
      vs[i] = true;
      m_test_connectivity(vs, i);
    }
  }
}

template <typename T> bool network<T>::connected() {
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

template <typename T> T network<T>::random_vertex(std::mt19937_64 &rng) {
  std::uniform_int_distribution<> unif(0, m_net.size());
  const unsigned int v = unif(rng);
  unsigned int index = 0;
  for (auto i : m_net) {
    if (v == index++) {
      return i.first;
    }
  }
  exit(EXIT_FAILURE);
}

template <typename T>
void network<T>::rgg(unsigned int order, double radius, std::mt19937_64 &rng) {
  std::uniform_real_distribution<> unif;
  m_net.clear();
  for (int i = 0; i < order; ++i) {
    T p(unif(rng), unif(rng));
    std::set<T> emps;
    m_net.insert(std::pair<T, std::set<T>>(p, emps));
  }
  for (auto i : m_net) {
    for (auto j : m_net) {
      if (distance(i.first, j.first) < radius) {
        add_edges(i.first, j.first);
      }
    }
  }
}

template <typename T> bool network<T>::has_vertex(const T &t) const {
  return m_net.find(t) != m_net.end();
}

template <typename T> bool network<T>::has_edge(const T &t0, const T &t1) {
  if (has_vertex(t0) && has_vertex(t1)) {
    return (m_net[t0].find(t1) != m_net[t0].end());
  } else {
    return false;
  }
}

template <typename T> bool network<T>::add_vertex(const T &t) {
  if (has_vertex(t)) {
    return false;
  } else {
    std::set<T> emps;
    m_net.insert(std::pair<T, std::set<T>>(t, emps));
    return true;
  }
}

template <typename T>
unsigned int network<T>::add_edge(const T &t0, const T &t1) {
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

template <typename T>
unsigned int network<T>::add_edges(const T &t0, const T &t1) {
  return add_edge(t0, t1) + add_edge(t1, t0);
}

template <typename T> std::set<T> network<T>::neighbors(const T &t) {
  return m_net[t];
}

template <typename T> std::set<T> network<T>::operator[](const T &t) {
  return m_net[t];
}

template <typename T>
typename std::map<T, std::set<T>>::iterator network<T>::begin() {
  return m_net.begin();
}

template <typename T>
typename std::map<T, std::set<T>>::iterator network<T>::end() {
  return m_net.end();
}

template <typename T>
typename std::map<T, std::set<T>>::const_iterator network<T>::begin() const {
  return m_net.begin();
}

template <typename T>
typename std::map<T, std::set<T>>::const_iterator network<T>::end() const {
  return m_net.end();
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const network<T> &net) {
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

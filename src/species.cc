#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include "wagner/tbranch.hh"
#include "wagner/network.hh"
#include "wagner/species.hh"
#include "wagner/point.hh"

namespace wagner {

species::species(size_t i, size_t ntraits)
  : tbranch(nullptr, nullptr, nullptr), id{i}, m_traits{std::vector<float>(ntraits, 0.0f)} {
  //
}

species::species(size_t i, std::vector<float> const& starting_traits)
  : tbranch(nullptr, nullptr, nullptr), id(i), m_traits{starting_traits} {
  //
}

auto species::num_traits() const -> size_t {
  return m_traits.size();
}

auto species::traits() -> std::vector<float>& {
  return m_traits;
}

auto species::operator[](size_t idx) -> float& {
  return m_traits[idx];
}

auto species::begin() const -> std::vector<float>::const_iterator {
  return m_traits.begin();
}

auto species::end() const -> std::vector<float>::const_iterator {
  return m_traits.end();
}

size_t species::size() const {
  return m_locations.size();
}

size_t species::num_groups() const {
  return m_groups;
}

bool species::is_in(const point &p) const {
  return m_locations.find(p) != m_locations.end();
}

bool species::extinct() const {
  return m_locations.size() == 0;
}

boost::container::flat_set<point> species::pop_group(int g) {
  boost::container::flat_set<point> gr;
  for (auto i : m_locations) {
    if (i.second == g) {
      gr.insert(i.first);
    }
  }
  for (auto i : m_locations) {
    if (gr.find(i.first) != gr.end()) {
      auto const p = i.first;
      m_locations.erase(p);
    }
  }
  return gr;
}

boost::container::flat_set<point> species::get_locations() const {
  boost::container::flat_set<point> l;
  for (auto i : m_locations) l.insert(i.first);
  return l;
}

size_t species::up_groups(network<point> &n) {
  size_t ngr = 0;
  for (auto i : m_locations) {
    m_locations[i.first] = -1;
  }
  for (auto i : m_locations) {
    if (i.second == -1) {
      m_locations[i.first] = ngr;
      m_grouping(i.first, ngr, n);
      ++ngr;
    }
  }
  m_groups = ngr;
  return ngr;
}

void species::m_grouping(const point &p, int gid, network<point> &n) {
  auto ns = n.neighbors(p);
  for (auto i : ns) {
    if ((m_locations.find(i) != m_locations.end()) && m_locations[i] == -1) {
      m_locations[i] = gid;
      m_grouping(i, gid, n);
    }
  }
}

void species::add_to(const point &p) {
  m_locations[p] = -1;
}

void species::add_to(const boost::container::flat_set<point> &ps) {
  for (auto p : ps) {
    add_to(p);
  }
}

void species::rmv_from(const point &p) {
  m_locations.erase(p);
}

size_t species::num_differences(const species &s) const {
  size_t delta = 0;
  for (int i = 0; i < m_traits.size(); ++i)
    delta += m_traits[i] != s.m_traits[i];
  return delta;
}

bool species::same_traits_as(const species &s) const {
  for (int i = 0; i < m_traits.size(); ++i) {
    if (m_traits[i] != s.m_traits[i]) {
      return false;
    }
  }
  return true;
}

size_t species::get_mrca(const species &s) const {
  tbranch *p = parent();
  boost::container::flat_set<tbranch *> parents;
  while (p != nullptr) {
    parents.insert(p);
    p = p->parent();
  }
  p = s.parent();
  assert(p != nullptr);
  while (parents.find(p) == parents.end()) {
    p = p->parent();
  }
  return p->end_date();
}

size_t species::get_mrca(const boost::container::flat_set<tbranch *> &ps) const {
  tbranch *p = parent();
  assert(p != nullptr && ps.size() > 0);
  while (ps.find(p) == ps.end()) {
    p = p->parent();
  }
  return p->end_date();
}

bool species::operator==(const species &s) const {
  return id == s.id;
}

bool species::operator!=(const species &s) const {
  return id != s.id;
}

bool species::operator<(const species &s) const {
  return id < s.id;
}

bool species::operator>(const species &s) const {
  return id > s.id;
}

boost::container::flat_set<point> species::operator&(species &s) const {
  boost::container::flat_set<point> l;
  boost::container::flat_set<point> l0 = get_locations();
  boost::container::flat_set<point> l1 = s.get_locations();
  for (auto i : l0) {
    if (l1.find(i) != l1.end()) {
      l.insert(i);
    }
  }
  return l;
}

std::string species::name() const {
  std::ostringstream o;
  o << "species" << id;
  return o.str();
}

std::string species::newick() const {
  std::ostringstream o;
  o << "species" << id << ':' << parent_distance();
  return o.str();
}

std::string species::get_info(size_t time) const {
  std::ostringstream oss;
  auto const locations = get_locations();
  oss << "<species> <id>" << id << "</id> <centroid>"
      << centroid(locations) << "</centroid> <locations>";
  for (auto i : m_locations) {
    oss << " <vertex><position>" << i.first << "</position><group>" << i.second
        << "</group></vertex>";
  }
  oss << "</locations> <traits>[";
  auto const n = m_traits.size();
  if (n) {
    oss << m_traits[0];
    for (auto i = 1u; i < n; ++i) oss << ", " << m_traits[i];
  }
  oss << "]</traits></species>";
  return oss.str();
}

std::ostream &operator<<(std::ostream &os, const species &s) {
  os << "<species> <id>" << s.id << "</id> </species>";
  return os;
}

}

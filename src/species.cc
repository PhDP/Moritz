#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include "wagner/common.hh"
#include "wagner/tbranch.hh"
#include "wagner/network.hh"
#include "wagner/species.hh"
#include "wagner/point.hh"

namespace wagner {

species::species(size_t i, size_t ntraits) noexcept
  : tbranch(nullptr, nullptr, nullptr), id{i}, m_traits{std::vector<float>(ntraits, 0.0f)} {
  //
}

species::species(size_t i, std::vector<float> const& starting_traits) noexcept
  : tbranch(nullptr, nullptr, nullptr), id(i), m_traits{starting_traits} {
  //
}

auto species::num_traits() const noexcept -> size_t {
  return m_traits.size();
}

auto species::traits() noexcept -> std::vector<float>& {
  return m_traits;
}

auto species::operator[](size_t idx) noexcept -> float& {
  return m_traits[idx];
}

auto species::begin() const noexcept -> std::vector<float>::const_iterator {
  return m_traits.begin();
}

auto species::end() const noexcept -> std::vector<float>::const_iterator {
  return m_traits.end();
}

auto species::size() const noexcept -> size_t {
  return m_locations.size();
}

auto species::num_groups() const noexcept -> size_t {
  return m_groups;
}

auto species::is_in(const point &p) const noexcept -> bool {
  return m_locations.find(p) != m_locations.end();
}

auto species::extinct() const noexcept -> bool {
  return m_locations.size() == 0;
}

auto species::pop_group(int g) noexcept -> set<point> {
  set<point> gr;
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

auto species::get_locations() const noexcept -> map<point, int> const& {
  return m_locations;
}

auto species::up_groups(network<point> &n) noexcept -> size_t {
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

auto species::m_grouping(const point &p, int gid, network<point> &n) noexcept -> void {
  auto ns = n.neighbors(p);
  for (auto i : ns) {
    if ((m_locations.find(i) != m_locations.end()) && m_locations[i] == -1) {
      m_locations[i] = gid;
      m_grouping(i, gid, n);
    }
  }
}

auto species::add_to(const point &p) noexcept -> void {
  m_locations[p] = -1;
}

auto species::add_to(const set<point> &ps) noexcept -> void {
  for (auto p : ps) {
    add_to(p);
  }
}

auto species::rmv_from(const point &p) noexcept -> void {
  m_locations.erase(p);
}

auto species::num_differences(const species &s) const noexcept -> size_t {
  size_t delta = 0;
  for (int i = 0; i < m_traits.size(); ++i)
    delta += m_traits[i] != s.m_traits[i];
  return delta;
}

auto species::same_traits_as(const species &s) const noexcept -> bool {
  for (int i = 0; i < m_traits.size(); ++i) {
    if (m_traits[i] != s.m_traits[i]) {
      return false;
    }
  }
  return true;
}

auto species::get_mrca(const species &s) const noexcept -> size_t {
  tbranch *p = parent();
  set<tbranch *> parents;
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

auto species::get_mrca(const set<tbranch *> &ps) const noexcept -> size_t {
  tbranch *p = parent();
  assert(p != nullptr && ps.size() > 0);
  while (ps.find(p) == ps.end()) {
    p = p->parent();
  }
  return p->end_date();
}

auto species::operator==(const species &s) const noexcept -> bool {
  return id == s.id;
}

auto species::operator!=(const species &s) const noexcept -> bool {
  return id != s.id;
}

auto species::operator<(const species &s) const noexcept -> bool {
  return id < s.id;
}

auto species::operator>(const species &s) const noexcept -> bool {
  return id > s.id;
}

auto species::operator&(species &s) const noexcept -> set<point> {
  set<point> l;
  auto l0 = get_locations();
  auto l1 = s.get_locations();
  for (auto const& i : l0) {
    if (l1.find(i.first) != l1.end()) {
      l.insert(i.first);
    }
  }
  return l;
}

auto species::name() const noexcept -> std::string {
  std::ostringstream o;
  o << "species" << id;
  return o.str();
}

auto species::newick() const noexcept -> std::string {
  std::ostringstream o;
  o << "species" << id << ':' << parent_distance();
  return o.str();
}

auto species::get_info(size_t time) const noexcept -> std::string {
  std::ostringstream oss;
  oss << "<species> <id>" << id << "</id> <centroid>"
      << centroid(m_locations) << "</centroid> <locations>";
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

auto operator<<(std::ostream &os, const species &s) noexcept -> std::ostream& {
  os << "<species> <id>" << s.id << "</id> </species>";
  return os;
}

}

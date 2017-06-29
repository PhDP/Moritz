#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include "wagner/common.hh"
#include "wagner/tbranch.hh"

namespace wagner {

tbranch::tbranch(tbranch *p, tbranch *l, tbranch *r) noexcept
    : m_parent(p), m_left(l), m_right(r) {
  m_end_date = 0;
}

tbranch::~tbranch() noexcept {
  delete m_left;
  delete m_right;
}

auto tbranch::parent() const noexcept -> tbranch* {
  return m_parent;
}

auto tbranch::left() const noexcept -> tbranch* {
  return m_left;
}

auto tbranch::right() const noexcept -> tbranch* {
  return m_right;
}

auto tbranch::set_parent(tbranch *t) noexcept -> void {
  m_parent = t;
}

auto tbranch::set_left(tbranch *t) noexcept -> void {
  m_left = t;
}

auto tbranch::set_right(tbranch *t) noexcept -> void {
  m_right = t;
}

auto tbranch::internal() const noexcept -> bool {
  return m_parent != nullptr && m_left != nullptr;
}

auto tbranch::root() const noexcept -> bool {
  return m_parent == nullptr;
}

auto tbranch::leaf() const noexcept -> bool {
  return m_left == nullptr; // Assumes the node is strictly binary.
}

auto tbranch::end_date() const noexcept -> size_t {
  return m_end_date;
}

auto tbranch::set_end_date(size_t date) noexcept -> void {
  m_end_date = date;
}

auto tbranch::max_end_date() const noexcept -> size_t {
  if (leaf()) {
    return m_end_date;
  } else {
    const int l_max = m_left->max_end_date();
    const int r_max = m_right->max_end_date();
    return max2(l_max, r_max);
  }
}

auto tbranch::parent_distance() const noexcept -> size_t {
  return (m_parent == nullptr) ? 0 : m_end_date - m_parent->end_date();
}

auto tbranch::total_distance() const noexcept -> size_t {
  return (m_parent == nullptr) ? 0 : parent_distance() + m_parent->total_distance();
}

auto tbranch::nodes() const noexcept -> size_t {
  return (m_parent == nullptr) ? 0 : 1 + m_parent->nodes();
}

auto tbranch::edges() const noexcept -> size_t {
  return (m_left == nullptr) ? 0 : 2 + m_left->edges() + m_right->edges();
}

auto tbranch::leaves() const noexcept -> size_t {
  return 1 + edges() / 2;
}

auto tbranch::strictly_binary() const noexcept -> bool {
  if (m_left == nullptr && m_right == nullptr)
    return true;
  else if ((m_left == nullptr && m_right != nullptr) || (m_left != nullptr && m_right == nullptr))
    return false;
  else
    return m_left->strictly_binary() && m_right->strictly_binary();
}

auto tbranch::level() const noexcept -> size_t {
  return leaf() ? 0 : 1 + max2(m_left->level(), m_right->level());
}

auto tbranch::newick() const noexcept -> std::string {
  std::ostringstream o;
  o << "(" << m_left->newick() << "," << m_right->newick()
    << "):" << parent_distance();
  if (root()) {
    o << ";";
  }
  return o.str();
}

}

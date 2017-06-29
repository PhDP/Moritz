#include <ostream>
#include <string>
#include <set>
#include <list>
#include "wagner/common.hh"
#include "wagner/speciestree.hh"
#include "wagner/tbranch.hh"
#include "wagner/species.hh"
#include "wagner/point.hh"
#include "wagner/n-sphere.hh"

namespace wagner {

speciestree::speciestree(std::vector<float> const& traits) noexcept {
  m_id_count = 0;
  species *s0 = new species(m_id_count++, traits);
  m_tips.insert(s0);
  m_start_date = 0;
  m_root = s0;
}

speciestree::~speciestree() noexcept {
  delete m_root;
}

auto speciestree::num_species() noexcept -> size_t {
  return m_tips.size();
}

auto speciestree::rmv_extinct(size_t date) noexcept -> boost::container::flat_set<species*> {
  boost::container::flat_set<species*> to_rmv;
  for (auto i : m_tips) {
    species *s = i;
    if (s->extinct()) {
      s->set_end_date(date);
      if (s == m_root) {
        m_root = nullptr;
      } else if (s->parent() == m_root) {
        tbranch *old_m_root = s->parent();
        tbranch *new_m_root = (old_m_root->left() == s) ? old_m_root->right()
                                                        : old_m_root->left();
        m_start_date += new_m_root->end_date() - m_start_date;
        m_root = new_m_root;
        m_root->set_parent(nullptr);
      } else {
        tbranch *parent = s->parent();
        tbranch *gramps = parent->parent();
        tbranch *other =
            (s == parent->left()) ? parent->right() : parent->left();
        other->set_parent(gramps);
        if (gramps->left() == parent) {
          gramps->set_left(other);
        } else {
          gramps->set_right(other);
        }
      }
      to_rmv.insert(i);
    }
  }

  for (auto i : to_rmv) {
    m_tips.erase(i);
  }
  return to_rmv;
}

auto speciestree::speciate(species* p, size_t date) noexcept -> species* {
  species *s0 = p;
  tbranch *new_parent = new tbranch(s0->parent(), nullptr, nullptr);
  new_parent->set_end_date(date);

  // If the species undergoing speciation has a parent:
  if (s0->parent() != nullptr) {
    if (s0->parent()->left() == s0) {
      s0->parent()->set_left(new_parent);
    } else {
      s0->parent()->set_right(new_parent);
    }
  } else {
    m_start_date += date;
  }

//  species *s1 = new species(m_id_count++);
  species *s1 = new species(m_id_count++, p->traits());
  s1->set_parent(new_parent);

  new_parent->set_left(s0);
  new_parent->set_right(s1);
  s0->set_parent(new_parent);

  // Change the m_root if the species undergoing speciation is also the m_root
  // of the tree
  if (m_tips.size() == 1) {
    m_root = new_parent;
  }

  // Add new species
  m_tips.insert(s1);
  return s1;
}

auto speciestree::stop(size_t date) noexcept -> void {
  for (auto i : m_tips) {
    i->set_end_date(date);
  }
}

auto speciestree::newick() const noexcept -> std::string {
  return (m_root == nullptr) ? ";" : m_root->newick();
}

auto speciestree::begin() noexcept -> boost::container::flat_set<species*>::iterator {
  return m_tips.begin();
}

auto speciestree::end() noexcept -> boost::container::flat_set<species*>::iterator {
  return m_tips.end();
}

auto speciestree::begin() const noexcept -> boost::container::flat_set<species*>::const_iterator {
  return m_tips.begin();
}

auto speciestree::end() const noexcept -> boost::container::flat_set<species*>::const_iterator {
  return m_tips.end();
}

auto operator<<(std::ostream &os, const speciestree &t) noexcept -> std::ostream& {
  os << t.newick();
  return os;
}

}

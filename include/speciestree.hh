#ifndef WAGNER_SPECIESTREE_HH_
#define WAGNER_SPECIESTREE_HH_

#include <ostream>
#include <string>
#include <set>
#include "tbranch.hh"
#include "species.hh"
#include "point.hh"

namespace wagner {

/** An object to store species and their phylogeny. */
class speciestree {
  tbranch* m_root;              // Root
  std::set<species*> m_tips;    // The tips of the tree (the extant species).
  size_t m_start_date;          // Start date of the tree.
  size_t m_id_count;            // Counter to name species.

public:
  /** Basic constructor. Creates a species and place it at the root. */
  speciestree();

  /** Basic destructor. */
  ~speciestree();

  /** Number of species in the tree. */
  auto num_species() -> size_t;

  /** Remove extinct species. */
  auto rmv_extinct(size_t date) -> std::set<species*>;

  /** Speciate. */
  auto speciate(species *parent, size_t date) -> species*;

  /** Set the date for extant species. */
  auto stop(size_t date) -> void;

  /** Return the tree in Newick format. */
  auto newick() const -> std::string;

  // Iterate the tips of the tree (the extant species):
  auto begin() -> std::set<species*>::iterator;
  auto end() -> std::set<species*>::iterator;
  auto begin() const -> std::set<species*>::const_iterator;
  auto end() const -> std::set<species*>::const_iterator;

  /** Return the tree in Newick format. */
  friend auto operator<<(std::ostream &os, const speciestree &t) -> std::ostream&;
};

}

#endif /* WAGNER_SPECIESTREE_HH_ */


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
private:
  // Root:
  tbranch *m_root;

  // The tips of the tree (the extant species):
  std::set<species *> m_tips;

  // Start date of the tree:
  unsigned int m_start_date;

  // Counter to name species:
  unsigned int m_id_count;

public:
  /** Basic constructor. Creates a species and place it at the root. */
  speciestree();

  /** Basic destructor. */
  ~speciestree();

  /** Number of species in the tree. */
  unsigned int num_species();

  /** Remove extinct species. */
  std::set<species *> rmv_extinct(unsigned int date);

  /** Speciate. */
  species *speciate(species *parent, unsigned int date);

  /** Set the date for extant species. */
  void stop(unsigned int date);

  /** Return the tree in Newick format. */
  std::string newick() const;

  // Iterate the tips of the tree (the extant species):
  std::set<species *>::iterator begin();
  std::set<species *>::iterator end();
  std::set<species *>::const_iterator begin() const;
  std::set<species *>::const_iterator end() const;

  /** Return the tree in Newick format. */
  friend std::ostream &operator<<(std::ostream &os, const speciestree &t);
};

}

#endif /* WAGNER_SPECIESTREE_HH_ */


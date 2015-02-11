#ifndef WAGNER_SPECIES_HH_
#define WAGNER_SPECIES_HH_

#include <iostream>
#include <set>
#include <vector>
#include "tbranch.hh"
#include "network.hh"
#include "point.hh"

namespace wagner {

/** Species as the leaf of a phylogenetic tree. */
class species : public tbranch {
private:
  std::vector<bool> m_traits;

  // Location/group map:
  std::map<point, int> m_locations;

  // Recursive function used to establish the groups:
  void m_grouping(const point &p, int gid, network<point> &n);

  // Number of groups:
  unsigned int m_groups;

public:
  /** Unique ID of the species. */
  const unsigned int id;

  /** Basic constructor. */
  species(unsigned int i);

  /** Basic destructor. */
  ~species();

  /** Return true if extinct. */
  bool extinct() const;

  /** Number of populations. **/
  unsigned int size() const;

  /** Number of groups. */
  unsigned int num_groups() const;

  /** Take a pointer to a spatial network, update the groups, return the number
   * of groups. */
  unsigned int up_groups(network<point> &n);

  /** Pop the gth group (that is: store the set of locations in a set, remove
   * them from this species and return it. */
  std::set<point> pop_group(int g);

  /** Return the set of locations. */
  std::set<point> get_locations() const;

  /** Test if the species is at the given location. */
  bool is_in(const point &p) const;

  /** Add a location to the species. */
  void add_to(const point &p);

  /** Add a location to the species. */
  void add_to(const std::set<point> &ps);

  /** Remove the species from a location. */
  void rmv_from(const point &p);

  /** Number of different traits from another species. */
  unsigned int num_differences(const species &s) const;

  /** Returns true if the species have the same traits. */
  bool same_traits_as(const species &s) const;

  /** Get most-recent-common-ancestor info. */
  unsigned int get_mrca(const species &s) const;

  /** Get most-recent-common-ancestor info given a set of parents. */
  unsigned int get_mrca(const std::set<tbranch *> &ps) const;

  /** Return the set of locations where both species are found (co-occurence).
   */
  std::set<point> operator&(species &s) const;

  /** Return the species' name. */
  std::string name() const;

  /** Get info on the species in XML format. */
  std::string get_info(unsigned int time) const;

  /** Return in newick format. */
  virtual std::string newick() const;

  // Use the ID to test equality and order:
  bool operator==(const species &s) const;
  bool operator!=(const species &s) const;
  bool operator<(const species &s) const;
  bool operator>(const species &s) const;

  /** Print name in XML format. */
  friend std::ostream &operator<<(std::ostream &os, const species &s);
};

}

#endif /* WAGNER_SPECIES_HH_ */

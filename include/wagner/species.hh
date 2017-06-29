#ifndef WAGNER_SPECIES_HH_
#define WAGNER_SPECIES_HH_

#include <iostream>
#include <set>
#include <vector>
#include "wagner/tbranch.hh"
#include "wagner/network.hh"
#include "wagner/point.hh"

namespace wagner {

/** Species as the leaf of a phylogenetic tree. */

class species : public tbranch {
  std::vector<float> m_traits;
  boost::container::flat_map<point, int> m_locations; // Location/group map.
  void m_grouping(const point &p, int gid, network<point> &n); // Recursive function used to establish the groups.
  size_t m_groups; // Number of groups.

 public:
  /** Unique ID of the species. */
  const size_t id;

  /** Basic constructor. */
  species(size_t i, size_t ntraits = 0);

  /** Creates a species with a starting set of traits. */
  species(size_t i, std::vector<float> const& starting_traits);

  /** Returns the number of traits. */
  auto num_traits() const -> size_t;

  /** Returns a reference to the trait vector. */
  auto traits() -> std::vector<float>&;

  /** Get the value of the nth trait. */
  auto operator[](size_t idx) -> float&;

  /** Iterate over the specie' traits. */
  auto begin() const -> std::vector<float>::const_iterator;

  /** End of the vector of traits. */
  auto end() const -> std::vector<float>::const_iterator;

  /** Return true if extinct. */
  auto extinct() const -> bool;

  /** Number of populations. **/
  auto size() const -> size_t;

  /** Number of groups. */
  auto num_groups() const -> size_t;

  /** Take a pointer to a spatial network, update the groups, return the number
   * of groups. */
  auto up_groups(network<point> &n) -> size_t;

  /** Pop the gth group (that is: store the set of locations in a set, remove
   * them from this species and return it. */
  auto pop_group(int g) -> boost::container::flat_set<point>;

  /** Return the set of locations. */
  auto get_locations() const -> boost::container::flat_map<point, int> const&;

  /** Test if the species is at the given location. */
  auto is_in(const point &p) const -> bool;

  /** Add a location to the species. */
  auto add_to(const point &p) -> void;

  /** Add a location to the species. */
  auto add_to(const boost::container::flat_set<point> &ps) -> void;

  /** Remove the species from a location. */
  auto rmv_from(const point &p) -> void;

  /** Number of different traits from another species. */
  auto num_differences(const species &s) const -> size_t;

  /** Returns true if the species have the same traits. */
  auto same_traits_as(const species &s) const -> bool;

  /** Get most-recent-common-ancestor info. */
  auto get_mrca(const species &s) const -> size_t;

  /** Get most-recent-common-ancestor info given a set of parents. */
  auto get_mrca(const boost::container::flat_set<tbranch*> &ps) const -> size_t;

  /** Return the set of locations where both species are found (co-occurence). */
  auto operator&(species &s) const -> boost::container::flat_set<point>;

  /** Return the species' name. */
  auto name() const -> std::string;

  /** Get info on the species in XML format. */
  auto get_info(size_t time) const -> std::string;

  /** Return in newick format. */
  virtual std::string newick() const;

  // Use the ID to test equality and order:
  auto operator==(const species &s) const -> bool;
  auto operator!=(const species &s) const -> bool;
  auto operator<(const species &s) const -> bool;
  auto operator>(const species &s) const -> bool;

  /** Print name in XML format. */
  friend auto operator<<(std::ostream &os, const species &s) -> std::ostream&;
};

}

#endif /* WAGNER_SPECIES_HH_ */

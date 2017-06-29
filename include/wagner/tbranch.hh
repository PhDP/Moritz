#ifndef WAGNER_TBRANCH_HH_
#define WAGNER_TBRANCH_HH_

#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include "wagner/common.hh"

namespace wagner {

/** An object for strictly binary trees: each node has either 2 or 0 children.
 */
class tbranch {
  // End of the branch:
  size_t m_end_date;
  // Parent of the node:
  tbranch* m_parent;
  // "Left" children:
  tbranch* m_left;
  // "Right" children:
  tbranch* m_right;

 public:
  /** Basic constructor. */
  tbranch(tbranch *p = nullptr, tbranch *l = nullptr, tbranch *r = nullptr) noexcept;

  /** Resursive destructor. */
  virtual ~tbranch();

  /** Return the parent. */
  auto parent() const noexcept -> tbranch*;

  /** Return the left child. */
  auto left() const noexcept -> tbranch*;

  /** Return the right child. */
  auto right() const noexcept -> tbranch*;

  /** Set the parent. */
  auto set_parent(tbranch *t) noexcept -> void;

  /** Set the left child. */
  auto set_left(tbranch *t) noexcept -> void;

  /** Set the right child. */
  auto set_right(tbranch *t) noexcept -> void;

  /** Return true if the node is a root. */
  auto root() const noexcept -> bool;

  /** Return true if the node is a leaf. */
  auto leaf() const noexcept -> bool;

  /** Return true for internal nodes. */
  auto internal() const noexcept -> bool;

  /** A recursive algorithm to test if the tree is strictly binary. */
  auto strictly_binary() const noexcept -> bool;

  /** Distance between this node and its parent. */
  auto parent_distance() const noexcept -> size_t;

  /** Return the distance from the node to the beginning of the tree. */
  auto total_distance() const noexcept -> size_t;

  /** Return the number of nodes from the root to this node. */
  auto nodes() const noexcept -> size_t;

  /** Return the number of edges in the subtree. */
  auto edges() const noexcept -> size_t;

  /** Return the number of leaves (i.e.: species) in the subtree. */
  auto leaves() const noexcept -> size_t;

  /** Max end date in the subtree. */
  auto max_end_date() const noexcept -> size_t;

  /** The level of the node as defined by Huson et al. */
  auto level() const noexcept -> size_t;

  /** Return the end date. */
  auto end_date() const noexcept -> size_t;

  /** Set the end date. */
  auto set_end_date(size_t date) noexcept -> void;

  /** Recursive function to get the Newick format. */
  virtual std::string newick() const noexcept;
};

}

#endif

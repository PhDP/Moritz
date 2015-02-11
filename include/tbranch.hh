#pragma once

#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include "common.hh"

/** An object for strictly binary trees: each node has either 2 or 0 children.
 */
class tbranch {
private:
  // End of the branch:
  unsigned int m_end_date;
  // Parent of the node:
  tbranch *m_parent;
  // "Left" children:
  tbranch *m_left;
  // "Right" children:
  tbranch *m_right;

public:
  /** Basic constructor. */
  tbranch(tbranch *p = nullptr, tbranch *l = nullptr, tbranch *r = nullptr);

  /** Resursive destructor. */
  virtual ~tbranch();

  /** Return the parent. */
  tbranch *parent() const;

  /** Return the left child. */
  tbranch *left() const;

  /** Return the right child. */
  tbranch *right() const;

  /** Set the parent. */
  void set_parent(tbranch *t);

  /** Set the left child. */
  void set_left(tbranch *t);

  /** Set the right child. */
  void set_right(tbranch *t);

  /** Return true if the node is a root. */
  bool root() const;

  /** Return true if the node is a leaf. */
  bool leaf() const;

  /** Return true for internal nodes. */
  bool internal() const;

  /** A recursive algorithm to test if the tree is strictly binary. */
  bool strictly_binary() const;

  /** Distance between this node and its parent. */
  unsigned int distance() const;

  /** Return the distance from the node to the beginning of the tree. */
  unsigned int total_distance() const;

  /** Return the number of nodes from the root to this node. */
  unsigned int nodes() const;

  /** Return the number of edges in the subtree. */
  unsigned int edges() const;

  /** Return the number of leaves (i.e.: species) in the subtree. */
  unsigned int leaves() const;

  /** Max end date in the subtree. */
  unsigned int max_end_date() const;

  /** The level of the node as defined by Huson et al. */
  unsigned int level() const;

  /** Return the end date. */
  unsigned int end_date() const;

  /** Set the end date. */
  void set_end_date(unsigned int date);

  /** Recursive function to get the Newick format. */
  virtual std::string newick() const;
};

#ifndef WAGNER_MODEL_HH_
#define WAGNER_MODEL_HH_

#include <iostream>

namespace wagner {

// Models:
enum class model {
  neutral = 0,
  phylo_dist = 1,
  euclidean_traits = 2,
  fuzzy_traits = 3
};

inline auto operator<<(std::ostream& os, model const& m) -> std::ostream& {
  switch (m) {
    case model::neutral:
      os << "neutral";
      break;
    case model::phylo_dist:
      os << "phylogenetic distance";
      break;
    case model::euclidean_traits:
      os << "euclidean traits";
      break;
    case model::fuzzy_traits:
      os << "fuzzy traits";
      break;
  }
  return os;
}

}

#endif

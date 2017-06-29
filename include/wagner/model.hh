#ifndef WAGNER_MODEL_HH_
#define WAGNER_MODEL_HH_

#include <iostream>

namespace wagner {

// Models:
enum class model {
  wagner_neutral = 0,
  wagner_aleph = 1,
  wagner_logistic = 2,
  wagner_log_aleph = 3,
  wagner_traits = 4
};

inline auto operator<<(std::ostream& os, model const& m) -> std::ostream& {
  switch(m) {
    case model::wagner_neutral:
      os << "neutral";
      break;
    case model::wagner_aleph:
      os << "aleph";
      break;
    case model::wagner_logistic:
      os << "logistic";
      break;
    case model::wagner_log_aleph:
      os << "log aleph";
      break;
    case model::wagner_traits:
      os << "with traits";
      break;
  }
  return os;
}

}

#endif

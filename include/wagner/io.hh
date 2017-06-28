#ifndef WAGNER_IO_HH_
#define WAGNER_IO_HH_

#include <iostream>
#include <array>
#include <vector>

namespace std {

template<typename T, size_t N>
auto operator<<(ostream&os, array<T, N> const& arr) -> ostream& {
  os << '[';
  if (N) {
    os << arr[0];
    for (size_t i = 1; i < N; ++i) os << ", " << arr[i];
  }
  os << ']';
  return os;
}

template<typename T>
auto operator<<(ostream&os, vector<T> const& xs) -> ostream& {
  os << '[';
  if (!xs.empty()) {
    os << xs[0];
    for (size_t i = 1; i < xs.size(); ++i) os << ", " << xs[i];
  }
  os << ']';
  return os;
}

} /* end namespace std */

#endif

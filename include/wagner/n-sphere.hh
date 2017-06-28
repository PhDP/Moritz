#ifndef WAGNER_N_SPHERE_HH_
#define WAGNER_N_SPHERE_HH_

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

namespace wagner {

/**
  \brief Checks whether a sphere represented by the coordinates in 'sphere' lies
         in the n-sphere with a certain radius.
 */
inline bool
in_sphere(std::vector<double> const& sphere, double radius = 0.5) noexcept {
  auto sum = 0.0;
  for (auto x : sphere) sum += x * x;
  return sum < radius * radius;
}

/**
  \brief Generates the coordinates of a n-dimentional sphere within a given radius
 */
inline std::vector<double>
random_n_sphere(std::mt19937_64& rng, size_t n, double radius = 0.5) noexcept {
  auto sphere = std::vector<double>(n, 0.0);
  auto dist = std::uniform_real_distribution<double>{-radius, radius};
  do {
    for (size_t i = 0; i < n; ++i)
      sphere[i] = dist(rng);
  } while (!in_sphere(sphere, radius));
  return sphere;
}

/** \brief Euclidean distance between two vectors. */
inline double
euclidean_distance(std::vector<double> const& xs, std::vector<double> const& ys) noexcept {
  double sum = 0.0;
  size_t const n = std::min(xs.size(), ys.size());
  for (auto i = 0u; i < n; ++i) {
    double const sub = xs[i] - ys[i];
    sum += sub * sub;
  }
  return std::sqrt(sum);
}

/** \brief Apply white noise to sphere, making sure it remains within the sphere. */
inline void
white_noise(std::vector<double>& xs, std::mt19937_64& rng, std::normal_distribution<>& d, double radius = 0.5) {
  auto const n = xs.size();
  for (;;) {
    auto new_xs = xs;
    for (auto i = 0u; i < n; ++i) new_xs[i] = xs[i] + d(rng);

    if (in_sphere(new_xs, radius)) {
      for (auto i = 0u; i < n; ++i) xs[i] = new_xs[i];
      return;
    }
  }
}

} /* end namespace wagner */

#endif

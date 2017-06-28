#include "gtest/gtest.h"
#include "wagner/n-sphere.hh"

TEST(WagnerNSphere, DistancesBetweenRandomSpheresBelow1) {
  auto rng = std::mt19937_64{42};
  for (auto i = 0; i < 1000; ++i) {
    auto const x = wagner::random_n_sphere(rng, 10);
    auto const y = wagner::random_n_sphere(rng, 10);
    EXPECT_TRUE(wagner::euclidean_distance(x, y) >= 0.0);
    EXPECT_TRUE(wagner::euclidean_distance(x, y) <= 1.0);
  }
}

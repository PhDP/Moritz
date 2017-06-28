/*! \file

  \brief Sebastiano Vigna's simple random number generator used to seed
         xorshift.
 */
#ifndef WAGNER_SPLITMIX64_HH_
#define WAGNER_SPLITMIX64_HH_

#include <cstdint>
#include "wagner/rng.hh"

namespace wagner {

/**
  \brief Returns a random number with the splitmix64 algorithm.
 */
inline auto splitmix64(uint64_t seed) -> uint64_t {
  uint64_t z = (seed += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

/**
  \brief Fills an array of random numbers with the splitmix64 algorithm.

  \param  seed    Initial number to seed the random number generator.
  \param  arr     Array to be filled.
  \param  nmemb   Number of elements in the array.
 */
inline auto splitmix64_arr(uint64_t seed, uint64_t *arr, size_t nmemb) -> void {
  while (nmemb-- > 0) {
    seed = splitmix64(seed);
    *arr++ = seed;
  }
}

class mix64 : public rng {
  uint64_t m_state;

 public:
  mix64(uint64_t seed) noexcept
    : m_state(seed) {
  }

  /**
    \brief Returns the next 64-bit integer.
   */
  auto next() noexcept -> uint64_t {
    m_state = splitmix64(m_state);
    return m_state;
  }

  /**
    \brief Returns the next number in the [0, 1) range.
   */
  auto next_double() noexcept -> double {
     const union { uint64_t i; double d; } u = {
      .i = UINT64_C(0x3FF) << 52 | next() >> 12
    };
    return u.d - 1.0;
  }
};

} /* end namespace */

#endif

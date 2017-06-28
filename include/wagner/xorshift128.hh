/*! \file

  \brief Sebastiano Vigna's xoroshiro128+ generator for random number.
 */
#ifndef WAGNER_XS128_HH_
#define WAGNER_XS128_HH_

#include <array>
#include <cstdint>
#include <cmath>
#include "splitmix64.hh"
#include "wagner/rng.hh"

#define WAGNER_ROTL(x, k)  ((x) << (k)) | ((x) >> (64 - (k)));

namespace wagner {

class xs128 : public rng {
  std::array<uint64_t, 2> m_state;

 public:
  /**
    \brief Initializes a xoroshiro128+ generator with a seed and the splitmix64
          generator.
   */
  xs128(uint64_t seed) noexcept {
    m_state[0] = splitmix64(seed);
    m_state[1] = splitmix64(m_state[0]);
  }

  /**
    \brief Returns the next 64-bit integer.
   */
  auto next() noexcept -> uint64_t {
    uint64_t const s0 = m_state[0];
    uint64_t s1 = m_state[1];
    uint64_t const result = s0 + s1;
    s1 ^= s0;
    m_state[0] = WAGNER_ROTL(s0, 55) ^ s1 ^ (s1 << 14);
    m_state[1] = WAGNER_ROTL(s1, 36);
    return result;
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

  /**
    \brief This is the jump function for the generator. It is equivalent
           to 2^512 calls to next(); it can be used to generate 2^512
           non-overlapping subsequences for parallel computations.
   */
  auto jump() noexcept -> void {
    static uint64_t const JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

    uint64_t s0 = 0, s1 = 0;
    for (auto i = 0; i < sizeof(JUMP) / sizeof(*JUMP); ++i) {
      for (auto b = 0; b < 64; b++) {
        if (JUMP[i] & 1ULL << b) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
        }
        next();
      }
    }
    m_state[0] = s0;
    m_state[1] = s1;
  }

} /* end namespace wagner */

#endif

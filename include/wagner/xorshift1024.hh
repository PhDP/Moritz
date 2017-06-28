/*! \file

  \brief Sebastiano Vigna's xorshift1024* algorithm for random numbers.
 */
#ifndef WAGNER_XS1024_HH_
#define WAGNER_XS1024_HH_

#include <array>
#include <cstdint>
#include <cmath>
#include "wagner/splitmix64.hh"
#include "wagner/rng.hh"

namespace wagner {

class xs1024 : public rng {
  std::array<uint64_t, 16> m_state;
  int64_t m_p;

 public:

  /**
    \brief Initializes a xoroshiro128+ generator with a seed and the splitmix64
          generator.
   */
  xs1024(uint64_t seed) noexcept : m_p(0) {
    for (auto i = 0; i < 16; ++i) {
      seed = splitmix64(seed);
      m_state[i] = seed;
    }
  }

  /**
    \brief Returns the next 64-bit integer.
   */
  auto next() noexcept -> uint64_t {
    uint64_t const s0 = m_state[m_p];
    uint64_t s1 = m_state[m_p = (m_p + 1) & 15];
    s1 ^= s1 << 31; // a
    m_state[m_p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b, c
    return m_state[m_p] * UINT64_C(1181783497276652981);
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
    static uint64_t const JUMP[] = { 0x84242f96eca9c41d,
      0xa3c65b8776f96855, 0x5b34a39f070b5837, 0x4489affce4f31a1e,
      0x2ffeeb0a48316f40, 0xdc2d9891fe68c022, 0x3659132bb12fea70,
      0xaac17d8efa43cab8, 0xc4cb815590989b13, 0x5ee975283d71c93b,
      0x691548c86c1bd540, 0x7910c41d10a1e6a5, 0x0b5fc64563b3e2a8,
      0x047f7684e9fc949d, 0xb99181f2d8f685ca, 0x284600e3f30e38c3
    };

    uint64_t t[16] = { 0 };
    for (auto i = 0; i < sizeof(JUMP) / sizeof(*JUMP); ++i) {
      for (auto b = 0; b < 64; ++b) {
        if (JUMP[i] & 1ULL << b) {
          for (auto j = 0; j < 16; ++j)
            t[j] ^= m_state[(j + m_p) & 15];
        }
        next();
      }
    }
    for (auto j = 0; j < 16; ++j) m_state[(j + m_p) & 15] = t[j];
  }
};

} /* end namespace wagner */

#endif

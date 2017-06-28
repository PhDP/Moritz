/*! \file

  \brief Abstract base class for random number generators.
 */
#ifndef WAGNER_RNG_HH_
#define WAGNER_RNG_HH_

#include <cmath>

namespace wagner {

class rng {
 public:
  /**
    \brief Returns the next 64-bit integer.
   */
  virtual uint64_t next() noexcept = 0;

  /**
    \brief Returns the next number in the [0, 1) range.
   */
  virtual double next_double() noexcept = 0;

  /**
    \brief Returns a double from the exponential distribution (between 0 and 1).
    \return          An exponential random variable.
   */
  auto exponential() -> double {
    double x = next_double();
    while (x == 0.0) x = next_double();
    return -std::log(x);
  }

  /**
    \brief Return a double from the normal distribution.
    \return          An exponential random variable.
   */
  auto normal() -> double {
    double n0, n1, s;
    do {
      n0 = 2.0 * next_double() - 1.0;
      n1 = 2.0 * next_double() - 1.0;
      s = n0 * n0 + n1 * n1;
    } while (s >= 1 || s == 0);
    return n0 * std::sqrt(-2.0 * std::log(s) / s);
  }

  /**
    \brief Generates two numbers in the normal distribution.
    \param n0        Storage for the second normal number.
    \param n1        Storage for the second normal number.
   */
  auto normals(double *n0, double *n1) -> void {
    double s;
    do {
      *n0 = 2.0 * next_double() - 1.0;
      *n1 = 2.0 * next_double() - 1.0;
      s = (*n0) * (*n0) + (*n1) * (*n1);
    } while (s >= 1 || s == 0);
    s = std::sqrt(-2.0 * std::log(s) / s);
    *n0 *= s;
    *n1 *= s;
  }

  /**
    \brief Returns an integer from a poisson distribution given lambda.

    \param r         The generator.
    \param lambda    The lambda parameter of the poisson distribution.
    \return          A number in the poisson distribution.
   */
  auto poisson(double lambda) -> int {
    double const l = std::pow(2.71828182845904523536, -lambda);
    double p = 1.0;
    int k = 0;
    do {
      ++k;
      p *= next_double();
    } while (p > l);
    return k - 1;
  }

};

} /* end namespace */

#endif

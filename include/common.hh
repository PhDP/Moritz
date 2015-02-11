#pragma once

#include <cmath>

/** Mathematical constant e. */
#define math_e 2.71828182845904523536

/** Mathematical constant pi. */
#define math_pi 3.14159265358979323846

/** Log 2 (very important in information theory). */
#define log2(x) (log(x) / log(2.0))

/** Check if a number is a power of two. */
#define power_of_two(n) (((n) != 0) && !((n) & ((n)-1)))

/** Cubic root. */
#define cbrt(x) (pow((x), 1.0 / 3.0))

/** Max of two values. */
#define max2(a, b) ((a) > (b) ? (a) : (b))

/** Min of two values. */
#define min2(a, b) ((a) < (b) ? (a) : (b))

/** Max of three values. */
#define max3(a, b, c) (max2(a, b) > (c) ? max2(a, b) : (c))

/** Min of three values. */
#define min3(a, b, c) (min2(a, b) < (c) ? min2(a, b) : (c))

/** Max of four values. */
#define max4(a, b, c, d) (max3(a, b, c) > (d) ? max3(a, b, c) : (d))

/** Min of four values. */
#define min4(a, b, c, d) (min3(a, b, c) < (d) ? min3(a, b, c) : (d))

/** Max of five values. */
#define max5(a, b, c, d, e) (max4(a, b, c, d) > (e) ? max4(a, b, c, d) : (e))

/** Min of five values. */
#define min5(a, b, c, d, e) (min4(a, b, c, d) < (e) ? min4(a, b, c, d) : (e))

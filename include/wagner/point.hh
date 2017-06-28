#ifndef WAGNER_POINT_HH_
#define WAGNER_POINT_HH_

#include <iostream>
#include <cmath>
#include <set>

namespace wagner {

/** An immutable two-dimensional point. */
struct point {
  /** x-coordinate. */
  const double x;

  /** y-coordinate. */
  const double y;

  /** Constructor. */
  point(double x_, double y_);

  /** Copy constructor. */
  point(const point &p);

  /** Basic destructor (although there's nothing to destroy). */
  ~point();

  /** Euclidean distance to another point. */
  auto distance_to(const point &p) const -> double;

  /** Return the middle point. */
  auto mid_point(const point &p) const -> point;

  /** Add points. */
  auto operator+(const point &p) const -> point;

  /** Substract points. */
  auto operator-(const point &p) const -> point;

  /** Multiply points. */
  auto operator*(const point &p) const -> point;

  /** Divide points. */
  auto operator/(const point &p) const -> point;

  /** Test equality of x and y. */
  auto operator==(const point &p) const -> bool;

  /** Look at x0 < x1 and then y0 < y1. */
  auto operator<(const point &p) const -> bool;

  /** Return the point in XML format. */
  auto xml() const -> std::string;

  /** Print point. */
  friend auto operator<<(std::ostream &os, const point &p) -> std::ostream&;
};

/** Compute the distance in Euclidean space between two points. */
inline auto euclidean_distance(const point &p1, const point &p2) -> double {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}

/** Compute the midpoint. */
inline auto mid_point(const point &p1, const point &p2) -> point {
  return point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}

/** Find the centroid of a set of points. */
inline auto centroid(const std::set<point> &ps) -> point {
  double x_ = 0.0;
  double y_ = 0.0;

  for (auto p : ps) {
    x_ += p.x;
    y_ += p.y;
  }
  x_ /= ps.size();
  y_ /= ps.size();

  return point(x_, y_);
}

}

#endif /* WAGNER_POINT_H_ */

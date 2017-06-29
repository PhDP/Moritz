#ifndef WAGNER_POINT_HH_
#define WAGNER_POINT_HH_

#include <iostream>
#include <cmath>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

namespace wagner {

// using point = std::tuple<double, double>;

/** A two-dimensional point. */
struct point {
  /** x-coordinate. */
  double x;

  /** y-coordinate. */
  double y;

  /** Constructor. */
  point(double x_, double y_) noexcept;

  /** Euclidean distance to another point. */
  auto distance_to(const point &p) const noexcept -> double;

  /** Return the middle point. */
  auto mid_point(const point &p) const noexcept -> point;

  /** Add points. */
  auto operator+(const point &p) const noexcept -> point;

  /** Substract points. */
  auto operator-(const point &p) const noexcept -> point;

  /** Multiply points. */
  auto operator*(const point &p) const noexcept -> point;

  /** Divide points. */
  auto operator/(const point &p) const noexcept -> point;

  /** Test equality of x and y. */
  auto operator==(const point &p) const noexcept -> bool;

  /** Look at x0 < x1 and then y0 < y1. */
  auto operator<(const point &p) const noexcept -> bool;

  /** Return the point in XML format. */
  auto xml() const noexcept -> std::string;

  /** Print point. */
  friend auto operator<<(std::ostream &os, const point &p) noexcept -> std::ostream&;
};

/** Compute the distance in Euclidean space between two points. */
inline auto euclidean_distance(const point &p1, const point &p2) noexcept -> double {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}

/** Compute the midpoint. */
inline auto mid_point(const point &p1, const point &p2) noexcept -> point {
  return point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}

/** Find the centroid of a set of points. */
inline auto centroid(const boost::container::flat_set<point> &ps) noexcept -> point {
  double x_ = 0.0;
  double y_ = 0.0;

  for (auto const& p : ps) {
    x_ += p.x;
    y_ += p.y;
  }
  x_ /= ps.size();
  y_ /= ps.size();

  return point(x_, y_);
}

/** Find the centroid of a set of points. */
inline auto centroid(const boost::container::flat_map<point, int> &ps) noexcept -> point {
  double x_ = 0.0;
  double y_ = 0.0;

  for (auto const& p : ps) {
    x_ += p.first.x;
    y_ += p.first.y;
  }
  x_ /= ps.size();
  y_ /= ps.size();

  return point(x_, y_);
}

}

#endif /* WAGNER_POINT_H_ */

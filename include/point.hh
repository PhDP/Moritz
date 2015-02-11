#ifndef WAGNER_POINT_H_
#define WAGNER_POINT_H_

#include <iostream>
#include <cmath>
#include <set>

namespace wagner {

/** An immutable two-dimensional point. */
class point {
public:
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
  double distance_to(const point &p) const;

  /** Return the middle point. */
  point mid_point(const point &p) const;

  /** Add points. */
  point operator+(const point &p) const;

  /** Substract points. */
  point operator-(const point &p) const;

  /** Multiply points. */
  point operator*(const point &p) const;

  /** Divide points. */
  point operator/(const point &p) const;

  /** Test equality of x and y. */
  bool operator==(const point &p) const;

  /** Look at x0 < x1 and then y0 < y1. */
  bool operator<(const point &p) const;

  /** Return the point in XML format. */
  std::string xml() const;

  /** Print point. */
  friend std::ostream &operator<<(std::ostream &os, const point &p);
};

/** Compute the distance in Euclidean space between two points. */
inline double euclidean_distance(const point &p1, const point &p2) {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}

/** Compute the midpoint. */
inline point mid_point(const point &p1, const point &p2) {
  return point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}

/** Find the centroid of a set of points. */
inline point centroid(const std::set<point> &ps) {
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

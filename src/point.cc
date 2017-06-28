#include <iostream>
#include <sstream>
#include <cmath>
#include "wagner/point.hh"

namespace wagner {

point::point(double x_, double y_) : x(x_), y(y_) {
  // ...
}

double point::distance_to(const point &p) const {
  return hypot(x - p.x, y - p.y);
}

point point::mid_point(const point &p) const {
  return point((x + p.x) / 2.0, (y + p.y) / 2.0);
}

point point::operator+(const point &p) const {
  return point(x + p.x, y + p.y);
}

point point::operator-(const point &p) const {
  return point(x - p.x, y - p.y);
}

point point::operator*(const point &p) const {
  return point(x * p.x, y * p.y);
}

point point::operator/(const point &p) const {
  return point(x / p.x, y / p.y);
}

bool point::operator==(const point &p) const {
  return (x == p.x) && (y == p.y);
}

bool point::operator<(const point &p) const {
  return (x != p.x) ? x < p.x : y < p.y;
}

std::string point::xml() const {
  std::ostringstream oss;
  oss << "<pt><x>" << x << "</x><y>" << y << "</y></pt>";
  return oss.str();
}

std::ostream &operator<<(std::ostream &os, const point &p) {
  os << '(' << p.x << ", " << p.y << ')';
  return os;
}

}

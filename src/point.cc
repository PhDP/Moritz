#include <iostream>
#include <sstream>
#include <cmath>
#include "wagner/point.hh"

namespace wagner {

point::point(double x_, double y_) noexcept : x(x_), y(y_) {
  // ...
}

double point::distance_to(const point &p) const noexcept {
  return hypot(x - p.x, y - p.y);
}

point point::mid_point(const point &p) const noexcept {
  return point((x + p.x) / 2.0, (y + p.y) / 2.0);
}

point point::operator+(const point &p) const noexcept {
  return point(x + p.x, y + p.y);
}

point point::operator-(const point &p) const noexcept {
  return point(x - p.x, y - p.y);
}

point point::operator*(const point &p) const noexcept {
  return point(x * p.x, y * p.y);
}

point point::operator/(const point &p) const noexcept {
  return point(x / p.x, y / p.y);
}

bool point::operator==(const point &p) const noexcept {
  return (x == p.x) && (y == p.y);
}

bool point::operator<(const point &p) const noexcept {
  return (x != p.x) ? x < p.x : y < p.y;
}

auto point::xml() const noexcept -> std::string {
  std::ostringstream oss;
  oss << "<pt><x>" << x << "</x><y>" << y << "</y></pt>";
  return oss.str();
}

auto operator<<(std::ostream &os, const point &p) noexcept -> std::ostream& {
  os << '(' << p.x << ", " << p.y << ')';
  return os;
}

}

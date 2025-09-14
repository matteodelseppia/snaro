#include <limits>
#include <cstdlib>

#include "snaro/segment.hpp"

namespace snaro {

bool segment::vertical() const { return start.x == end.x; }

double segment::y_at(double x) const {
  if (vertical()) {
    return start.y; // arbitrary!
  }
  const double t = (end.y - start.y) / (end.x - start.x);
  return start.y + t * (x - start.x);
}

urpoint segment::urpoint_at(double x) const { return {x, y_at(x)}; }

std::optional<urpoint> segment::intersection(const ursegment& other) const {
  double x1 = start.x, y1 = start.y;
  double x2 = end.x, y2 = end.y;
  double x3 = other.start.x, y3 = other.start.y;
  double x4 = other.end.x, y4 = other.end.y;

  double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (std::abs(denom) < std::numeric_limits<double>::epsilon()) {
    return std::nullopt;
  }

  double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
  double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

  if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
    double ix = x1 + t * (x2 - x1);
    double iy = y1 + t * (y2 - y1);
    return urpoint{ix, iy};
  }
  return std::nullopt;
}

} // namespace snaro

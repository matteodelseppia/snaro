#include "snaro/segment"

namespace snaro {

bool segment::vertical() const {
  return start.x == end.x; 
}

double segment::y_at(double x) const {
  if (vertical()) {
    return start.y; // arbitrary!
  }
  const double t = (end.y - start.y) / (end.x - start.x);
  return start.y + t * (x - start.x);
}

urpoint segment::urpoint_at(double x) const {
  return {x, y_at(x)};
}

} // namespace snaro
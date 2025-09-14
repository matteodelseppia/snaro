#pragma once

#include <cmath>

#include "snaro/segment.hpp"

namespace snaro::details {

struct pixel {
  using column = int64_t;
  using row = int64_t;

  column x;
  row y;

  rpoint to_rpoint(double precision) const {
    return {x * precision, y * precision};
  }

  static pixel from_urpoint(const urpoint& p, double precision) {
    return {std::llround(p.x / precision), std::llround(p.y / precision)};
  }

  static pixel::column column_of(const urpoint& p, double precision) {
    return from_urpoint({p.x, 0.0}, precision).x;
  }
};

} // namespace snaro::details

#pragma once

#include <cstdint>
#include <cmath>

#include "snaro/segment.hpp"

namespace snaro::details {

struct pixel {
  using column = int64_t;
  using row = int64_t;
  
  column x;
  row y;

  static inline pixel from_urpoint(const urpoint& p, double precision) {
    return {std::llround(p.x / precision), std::llround(p.y / precision)};
  }

  static inline rpoint to_rpoint(const pixel& p, double precision) {
    return {p.x * precision, p.y * precision};
  }

  static inline pixel::column column_of(const urpoint& p, double precision) {
    return from_urpoint({p.x, 0.0}, precision).x;
  }
};

} // namespace snaro::details
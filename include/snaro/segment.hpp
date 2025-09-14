#pragma once

#include <cstddef>
#include <optional>

#include "snaro/point.hpp"

namespace snaro {

struct segment {
  using unrounded = segment;
  using rounded = segment;
  using id = std::size_t;
  point start;
  point end;
  id seg_id;

  bool vertical() const;
  double y_at(double x) const;
  urpoint urpoint_at(double x) const;
  std::optional<urpoint> intersection(const segment& other) const;
};

using ursegment = segment::unrounded;
using rsegment = segment::rounded;

} // namespace snaro

#pragma once

namespace snaro {

struct point {
  using unrounded = point;
  using rounded = point;
  double x;
  double y;
};

using urpoint = point::unrounded;
using rpoint = point::rounded;

} // namespace snaro

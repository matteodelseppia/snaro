#pragma once

#include "snaro/segment.hpp"

#include <limits>
#include <vector>

namespace snaro {

struct bentley_ottmann {
  struct intersection {
    ursegment seg1;
    ursegment seg2;
    urpoint position;
  };

  std::vector<intersection>
  operator()(const std::vector<ursegment>& segments) const;
  const double m_precision = std::numeric_limits<double>::epsilon();
};

} // namespace snaro

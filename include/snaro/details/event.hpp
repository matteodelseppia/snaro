#pragma once

#include <optional>

#include "snaro/segment.hpp"

namespace snaro::details {

struct event {
  using id = size_t;
  enum class type { start = 0, intersection = 1, end = 2 };

  id event_id;
  type event_type;
  urpoint position;
  ursegment segment;
  std::optional<ursegment> intersected;

  bool operator<(const event& other) const;
};

} // namespace snaro::details

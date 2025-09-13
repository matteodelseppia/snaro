#include "snaro/details/event.hpp"

namespace snaro::details {

bool event::operator<(const event &other) const {
  if (position.x != other.position.x) {
    return position.x < other.position.x;
  }
  if (position.y != other.position.y) {
    return position.y < other.position.y;
  }
  if (event_type != other.event_type) {
    return static_cast<int>(event_type) < static_cast<int>(other.event_type);
  }

  return event_id < other.event_id;
}

} // namespace snaro::details
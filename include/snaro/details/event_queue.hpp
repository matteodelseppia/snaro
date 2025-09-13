#pragma once

#include <set>
#include <vector>
#include <optional>

#include "snaro/details/event.hpp"

namespace snaro::details {

class event_queue {
public:
  explicit event_queue(double precision) : m_precision{precision} {}
  bool empty() const;
  std::optional<event> front() const;
  event::id insert(const event& event);
  std::optional<event> pop();
  std::set<event> pop_column();

private:
  const double m_precision;
  std::set<event> m_events;
  event::id m_next_id = 0;
};

} // snaro::details
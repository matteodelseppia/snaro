#include "snaro/details/event_queue.hpp"
#include "snaro/details/pixel.hpp"

namespace snaro::details {

bool event_queue::empty() const {
  return m_events.empty();
}

std::optional<event> event_queue::front() const {
  if (empty()) {
    return std::nullopt;
  }

  return *m_events.cbegin();
}

event::id event_queue::insert(const event& e) {
  event ev = e;
  ev.event_id = m_next_id++;
  m_events.insert(ev);
  return ev.event_id;
}

std::optional<event> event_queue::pop() {
  if (empty()) {
    return std::nullopt;
  }

  event peek = front().value();
  m_events.erase(m_events.cbegin());
  return peek;
}

std::set<event> event_queue::pop_column() {
  std::set<event> events_of_column;
  if (empty()) {
    return events_of_column;
  }

  const event peek = front().value();
  const pixel::column current_column = pixel::column_of(peek.position, m_precision);
  while (!empty()) {
    const event e = front().value();

    if (const pixel::column event_column = pixel::column_of(e.position, m_precision);
        event_column != current_column) {
      break;
    }

    events_of_column.insert(e);
    pop();
  }

  return events_of_column;
}

}  // namespace snaro::details
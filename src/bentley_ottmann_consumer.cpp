#include "snaro/details/bentley_ottmann_consumer.hpp"
#include "snaro/details/pixel.hpp"

namespace snaro::details {

std::vector<event> bentley_ottmann_consumer::process(std::set<event>&& batch) {
  if (batch.empty()) {
    return {};
  }

  std::vector<event> new_events;
  m_event_batch = std::move(batch);

  while (!m_event_batch.empty()) {
    const event e = *m_event_batch.cbegin();
    const double event_x = e.position.x;

    m_active_segments.set_current_x(event_x);
    m_event_batch.erase(m_event_batch.cbegin());

    switch (e.event_type) {
      case event::type::start:
        for (auto& ev: handle_start_event(e)) {
          new_events.push_back(ev);
        }
        break;

      case event::type::intersection:
        m_intersections.push_back(e);
        for (auto& ev: handle_intersection_event(e)) {
          new_events.push_back(ev);
        }
        break;

      case event::type::end:
        for (auto& ev: handle_end_event(e)) {
          new_events.push_back(ev);
        }
        break;
    }
  }

  return new_events;
}

std::vector<event> bentley_ottmann_consumer::try_enqueue_intersection(
    const event& current_event, const ursegment& seg1, const ursegment& seg2) {
  std::vector<event> next_batch_events;

  static const auto is_event_in_current_column = [&](const event& e) -> bool {
    return pixel::column_of(e.position, m_precision) ==
           pixel::column_of(current_event.position, m_precision);
  };

  if (const auto intersection_point = seg1.intersection(seg2)) {
    if (intersection_point->x > current_event.position.x) {
      const event intersection_event{m_next_event_id--,
                                     event::type::intersection,
                                     *intersection_point, seg1, seg2};

      if (is_event_in_current_column(intersection_event)) {
        m_event_batch.insert(intersection_event);
      } else {
        next_batch_events.push_back(intersection_event);
      }
    }
  }

  return next_batch_events;
}

std::vector<event>
bentley_ottmann_consumer::handle_start_event(const event& e) {
  std::vector<event> new_events;
  m_active_segments.insert(e.segment);

  const auto& seg = e.segment;

  if (const auto above = m_active_segments.get_above(seg)) {
    const auto tmp = try_enqueue_intersection(e, seg, *above);
    new_events.insert(new_events.end(), tmp.begin(), tmp.end());
  }

  if (const auto below = m_active_segments.get_below(seg)) {
    const auto tmp = try_enqueue_intersection(e, seg, *below);
    new_events.insert(new_events.end(), tmp.begin(), tmp.end());
  }

  return new_events;
}

std::vector<event>
bentley_ottmann_consumer::handle_intersection_event(const event& e) {
  std::vector<event> new_events;

  m_active_segments.swap(e.segment, *e.intersected);

  const auto& seg1 = e.segment;
  const auto& seg2 = *e.intersected;

  if (const auto above = m_active_segments.get_above(seg1)) {
    const auto tmp = try_enqueue_intersection(e, seg1, *above);
    new_events.insert(new_events.end(), tmp.begin(), tmp.end());
  }

  if (const auto below = m_active_segments.get_below(seg2)) {
    const auto tmp = try_enqueue_intersection(e, seg2, *below);
    new_events.insert(new_events.end(), tmp.begin(), tmp.end());
  }

  m_intersections.push_back(e);
  return new_events;
}

std::vector<event> bentley_ottmann_consumer::handle_end_event(const event& e) {
  std::vector<event> new_events;

  if (const auto above = m_active_segments.get_above(e.segment)) {
    if (const auto below = m_active_segments.get_below(e.segment)) {
      const auto tmp = try_enqueue_intersection(e, *above, *below);
      new_events.insert(new_events.end(), tmp.cbegin(), tmp.cend());
    }
  }

  m_active_segments.remove(e.segment);
  return new_events;
}

const std::vector<event>& bentley_ottmann_consumer::get_intersections() const {
  return m_intersections;
}

} // namespace snaro::details

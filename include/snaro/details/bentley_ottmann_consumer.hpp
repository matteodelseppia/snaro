#pragma once

#include "snaro/details/active_segment_queue.hpp"
#include "snaro/details/event.hpp"

namespace snaro::details {

class bentley_ottmann_consumer {
public:
  explicit bentley_ottmann_consumer(double precision) :
      m_precision(precision) {}
  std::vector<event> process(std::set<event>&& batch);
  const std::vector<event>& get_intersections() const;

private:
  std::vector<event> handle_start_event(const event& e);
  std::vector<event> handle_end_event(const event& e);
  std::vector<event> handle_intersection_event(const event& e);
  std::vector<event> try_enqueue_intersection(const event& current_event,
                                              const ursegment& seg1,
                                              const ursegment& seg2);

  const double m_precision;
  std::set<event> m_event_batch;
  std::vector<event> m_intersections;
  active_segment_queue m_active_segments;
  event::id m_next_event_id = -1;
};

} // namespace snaro::details

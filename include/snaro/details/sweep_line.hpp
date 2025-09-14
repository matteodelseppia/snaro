#pragma once

#include "snaro/details/event_queue.hpp"

namespace snaro::details {

class sweep_line {
public:
  sweep_line(const std::vector<ursegment>& segments, double precision) :
      m_segments(segments), m_precision(precision),
      m_events_queue{m_precision} {}

  template<typename event_consumer_type>
  void sweep(event_consumer_type& consumer) {
    while (!is_done()) {
      process_batch(consumer);
    }
  }

private:
  bool is_done() const { return m_events_queue.empty(); }

  template<typename event_consumer_type>
  void process_batch(event_consumer_type& consumer) {
    if (is_done())
      return;

    auto events = m_events_queue.pop_column();
    if (events.empty())
      return;

    std::vector<event> new_events = consumer.process(std::move(events));
    for (const auto& e: new_events) {
      m_events_queue.insert(e);
    }
  }

  const std::vector<ursegment>& m_segments;
  const double m_precision;
  event_queue m_events_queue;
};

} // namespace snaro::details

#include "snaro/details/active_segment_queue.hpp"

namespace snaro::details {

void active_segment_priority_manager::add_rule(const active_segment::id& higher,
                                               const active_segment::id& lower) {
  m_rules[higher].insert(lower);
  m_rules[lower].erase(higher);
}

bool active_segment_priority_manager::operator()(const active_segment::id& this_id,
                                                 const active_segment::id& that_id) const {
  if (m_rules.contains(this_id))
    return m_rules.at(this_id).contains(that_id);
  return false;
}

bool active_segment_comparator::operator()(const active_segment& this_seg,
                                           const active_segment& that_seg) const {
  const double this_y = this_seg.y_at(m_sweep_line_x);
  const double that_y = that_seg.y_at(m_sweep_line_x);

  if (this_y != that_y)
    return this_y < that_y;

  if (m_priority_manager(this_seg.seg_id, that_seg.seg_id))
    return true;

  if (m_priority_manager(that_seg.seg_id, this_seg.seg_id))
    return false;

  return this_seg.seg_id < that_seg.seg_id;
}

bool active_segment_queue::is_empty() const {
  return m_queue.empty();
}

bool active_segment_queue::set_current_x(double x) {
  if (x < m_sweep_line_x)
    return false;
  m_sweep_line_x = x;
  return true;
}

bool active_segment_queue::insert(const active_segment& segment) {
  if (m_queue.contains(segment))
    return false;
  m_queue.insert(segment);
  return true;
}

bool active_segment_queue::remove(const active_segment& segment) {
  if (!m_queue.contains(segment))
    return false;

  m_queue.erase(segment);
  return true;
}

bool active_segment_queue::swap(const active_segment& this_seg,
                                const active_segment& that_seg) {
  if (this_seg.seg_id == that_seg.seg_id)
    return false;

  const auto it_this = m_queue.find(this_seg);
  const auto it_that = m_queue.find(that_seg);

  if (it_this == m_queue.cend() || it_that == m_queue.cend())
    return false;

  m_queue.erase(it_this);
  m_queue.erase(it_that);

  if (m_comparator(this_seg, that_seg)) {
    m_priority_manager.add_rule(that_seg.seg_id, this_seg.seg_id);
  } else {
    m_priority_manager.add_rule(this_seg.seg_id, that_seg.seg_id);
  }

  m_queue.insert(this_seg);
  m_queue.insert(that_seg);
  return true;
}

std::optional<active_segment> active_segment_queue::get_above(const active_segment& segment) const {
  const auto it = m_queue.find(segment);
  if (it == m_queue.cend())
    return std::nullopt;
  const auto it_above = std::next(it);
  if (it_above == m_queue.cend())
    return std::nullopt;
  return *it_above;
}

std::optional<active_segment> active_segment_queue::get_below(const active_segment& segment) const {
  const auto it = m_queue.find(segment);
  if (it == m_queue.cend() || it == m_queue.cbegin())
    return std::nullopt;
  const auto it_below = std::prev(it);
  return *it_below;
}

} // namespace snaro::details

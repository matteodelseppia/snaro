#pragma once

#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "snaro/segment.hpp"

namespace snaro::details {

using active_segment = ursegment;

class active_segment_priority_manager {
public:
  void add_rule(const active_segment::id& higher,
                const active_segment::id& lower);
  bool operator()(const active_segment::id& this_id,
                  const active_segment::id& that_id) const;

private:
  std::unordered_map<active_segment::id, std::unordered_set<active_segment::id>>
      m_rules;
};

class active_segment_comparator {
public:
  active_segment_comparator(
      const active_segment_priority_manager& priority_override,
      const double& sweep_line_x) :
      m_priority_manager{priority_override}, m_sweep_line_x{sweep_line_x} {}

  bool operator()(const active_segment& this_seg,
                  const active_segment& that_seg) const;

private:
  const active_segment_priority_manager& m_priority_manager;
  const double& m_sweep_line_x;
};

class active_segment_queue {
public:
  active_segment_queue() :
      m_comparator{m_priority_manager, m_sweep_line_x}, m_queue{m_comparator} {}

  bool is_empty() const;
  std::optional<active_segment> get_above(const active_segment& segment) const;
  std::optional<active_segment> get_below(const active_segment& segment) const;
  bool set_current_x(double x);
  bool insert(const active_segment& segment);
  bool remove(const active_segment& segment);
  bool swap(const active_segment& this_seg, const active_segment& that_seg);

private:
  double m_sweep_line_x = 0.0;
  active_segment_priority_manager m_priority_manager;
  active_segment_comparator m_comparator;
  std::set<active_segment, active_segment_comparator> m_queue;
};


} // namespace snaro::details

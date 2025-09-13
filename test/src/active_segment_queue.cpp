#include <gtest/gtest.h>

#include "snaro/details/active_segment_queue.hpp"

using namespace snaro::details;

namespace active_segment_queue_tests {

// helper to create segments with predictable behavior
active_segment make_segment(int id, double x1, double y1, double x2, double y2) {
  active_segment seg{{x1, y1}, {x2, y2}};
  seg.seg_id = id; // assuming id is public or settable
  return seg;
}

TEST(active_segment_priority_manager, add_and_query_rules) {
  active_segment_priority_manager manager;

  manager.add_rule(1, 2);
  EXPECT_TRUE(manager(1, 2));
  EXPECT_FALSE(manager(2, 1));

  // cycles should be prevented but changing rules is allowed
  manager.add_rule(2, 1);
  EXPECT_FALSE(manager(1, 2));
  EXPECT_TRUE(manager(2, 1));
}

TEST(active_segment_comparator, compare_by_y) {
  const active_segment_priority_manager manager;
  constexpr double sweep_x = 1.0;
  const active_segment_comparator cmp(manager, sweep_x);

  // seg_a has y=1, seg_b has y=2 at x=1
  const active_segment seg_a = make_segment(1, 0, 1, 2, 1);
  const active_segment seg_b = make_segment(2, 0, 2, 2, 2);

  EXPECT_TRUE(cmp(seg_a, seg_b));
  EXPECT_FALSE(cmp(seg_b, seg_a));
}

TEST(active_segment_comparator, compare_by_priority_override) {
  active_segment_priority_manager manager;
  manager.add_rule(1, 2);

  constexpr double sweep_x = 1.0;
  const active_segment_comparator cmp(manager, sweep_x);

  // both horizontal at same y
  const active_segment seg_a = make_segment(1, 0, 1, 2, 1);
  const active_segment seg_b = make_segment(2, 0, 1, 2, 1);

  EXPECT_TRUE(cmp(seg_a, seg_b));
  EXPECT_FALSE(cmp(seg_b, seg_a));
}

TEST(active_segment_queue, insert_and_remove) {
  active_segment_queue queue;

  const auto seg = make_segment(1, 0, 0, 1, 1);
  EXPECT_TRUE(queue.insert(seg));
  EXPECT_FALSE(queue.insert(seg)); // duplicate not inserted

  EXPECT_TRUE(queue.remove(seg));
  EXPECT_FALSE(queue.remove(seg)); // already removed
}

TEST(active_segment_queue, sweep_line_progression) {
  active_segment_queue queue;
  EXPECT_TRUE(queue.set_current_x(5.0));
  EXPECT_FALSE(queue.set_current_x(4.0)); // cannot move backwards
}

TEST(active_segment_queue, get_above_and_below) {
  active_segment_queue queue;

  auto seg1 = make_segment(1, 0, 0, 1, 0); // y=0
  auto seg2 = make_segment(2, 0, 1, 1, 1); // y=1
  auto seg3 = make_segment(3, 0, 2, 1, 2); // y=2

  queue.insert(seg1);
  queue.insert(seg2);
  queue.insert(seg3);

  auto below = queue.get_below(seg2);
  ASSERT_TRUE(below.has_value());
  EXPECT_EQ(below->seg_id, 1);

  auto above = queue.get_above(seg2);
  ASSERT_TRUE(above.has_value());
  EXPECT_EQ(above->seg_id, 3);

  EXPECT_FALSE(queue.get_below(seg1).has_value());
  EXPECT_FALSE(queue.get_above(seg3).has_value());
}

TEST(active_segment_queue, swap_segments_priority) {
  active_segment_queue queue;

  const auto seg_a = make_segment(1, 0, 1, 2, 1);
  const auto seg_b = make_segment(2, 0, 1, 2, 1);

  queue.insert(seg_a);
  queue.insert(seg_b);

  // after swap, priority manager must register one rule
  EXPECT_TRUE(queue.swap(seg_a, seg_b));
  EXPECT_FALSE(queue.swap(seg_a, seg_a)); // cannot swap same id
}

TEST(active_segment_queue, is_empty_check) {
  active_segment_queue queue;
  EXPECT_TRUE(queue.is_empty());

  const auto seg = make_segment(1, 0, 0, 1, 1);
  queue.insert(seg);
  EXPECT_FALSE(queue.is_empty());
}

} // namespace active_segment_queue_tests
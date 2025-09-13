#include <gtest/gtest.h>

#include "snaro/details/event_queue.hpp"
#include "snaro/details/pixel.hpp"

using namespace snaro;
using namespace snaro::details;

static event make_event(double x, double y) {
  event e;
  e.position = {x, y};
  return e;
}

namespace event_queue_tests {

TEST(event_queue, empty_on_construction) {
  event_queue q{1.0};
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.front(), std::nullopt);
  EXPECT_EQ(q.pop(), std::nullopt);
  EXPECT_TRUE(q.pop_column().empty());
}

TEST(event_queue, add_and_front) {
  event_queue q{1.0};
  event e = make_event(1.0, 2.0);
  event::id id = q.insert(e);

  ASSERT_TRUE(q.front().has_value());
  EXPECT_EQ(q.front()->event_id, id);
  EXPECT_FALSE(q.empty());
}

TEST(event_queue, add_assigns_increasing_ids) {
  event_queue q{1.0};
  event e1 = make_event(0.0, 0.0);
  event e2 = make_event(1.0, 1.0);

  event::id id1 = q.insert(e1);
  event::id id2 = q.insert(e2);

  EXPECT_EQ(id1, 0);
  EXPECT_EQ(id2, 1);
}

TEST(event_queue, pop_returns_and_removes_front) {
  event_queue q{1.0};
  q.insert(make_event(0.0, 0.0));
  q.insert(make_event(1.0, 1.0));

  auto e1 = q.pop();
  ASSERT_TRUE(e1.has_value());
  auto e2 = q.pop();
  ASSERT_TRUE(e2.has_value());
  EXPECT_TRUE(q.empty());
}

TEST(event_queue, pop_column_returns_all_in_same_column) {
  event_queue q{1.0};
  q.insert(make_event(0.2, 0.0)); // column 0
  q.insert(make_event(0.3, 1.0)); // column 0
  q.insert(make_event(1.4, 2.0)); // column 1

  auto col0 = q.pop_column();
  ASSERT_EQ(col0.size(), 2);
  EXPECT_EQ(pixel::column_of(col0.cbegin()->position, 1.0), 0);
  EXPECT_EQ(pixel::column_of(col0.crbegin()->position, 1.0), 0);

  auto col1 = q.pop_column();
  ASSERT_EQ(col1.size(), 1);
  EXPECT_EQ(pixel::column_of(col1.cbegin()->position, 1.0), 1);

  EXPECT_TRUE(q.empty());
}

TEST(event_queue, pop_column_respects_precision) {
  event_queue q{0.5};
  q.insert(make_event(0.1, 0.0)); // column 0
  q.insert(make_event(0.2, 1.0)); // column 0
  q.insert(make_event(0.6, 2.0)); // column 1

  auto col0 = q.pop_column();
  EXPECT_EQ(col0.size(), 2);

  auto col1 = q.pop_column();
  EXPECT_EQ(col1.size(), 1);
}

} // namespace event_queue_tests

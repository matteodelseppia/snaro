#include <gtest/gtest.h>

#include "snaro/details/event.hpp"

using namespace snaro::details;

namespace event_ordering_tests {

struct event_ordering : public ::testing::Test {
  static event make_event(int x, int y, int type, int id) {
    event e;
    e.position.x = x;
    e.position.y = y;
    e.event_type = static_cast<decltype(e.event_type)>(type);
    e.event_id = id;
    return e;
  }
};

TEST_F(event_ordering, orders_by_x_coordinate) {
  const auto e1 = make_event(1, 0, 0, 0);
  const auto e2 = make_event(2, 0, 0, 0);
  EXPECT_TRUE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
}

TEST_F(event_ordering, orders_by_y_when_x_equal) {
  const auto e1 = make_event(1, 1, 0, 0);
  const auto e2 = make_event(1, 2, 0, 0);
  EXPECT_TRUE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
}

TEST_F(event_ordering, orders_by_event_type_when_xy_equal) {
  const auto e1 = make_event(1, 1, 0, 0);
  const auto e2 = make_event(1, 1, 1, 0);
  EXPECT_TRUE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
}

TEST_F(event_ordering, orders_by_id_when_xy_and_type_equal) {
  const auto e1 = make_event(1, 1, 0, 1);
  const auto e2 = make_event(1, 1, 0, 2);
  EXPECT_TRUE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
}

TEST_F(event_ordering, equal_events_are_not_less) {
  const auto e1 = make_event(1, 1, 0, 1);
  const auto e2 = make_event(1, 1, 0, 1);
  EXPECT_FALSE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
}

} // namespace event_ordering_tests

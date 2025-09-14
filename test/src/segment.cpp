#include <gtest/gtest.h>

#include "snaro/segment.hpp"

using namespace snaro;

namespace segment_tests {

TEST(segment, is_vertical) {
  segment s1{{0, 0}, {0, 5}};
  EXPECT_TRUE(s1.vertical());

  segment s2{{0, 0}, {5, 0}};
  EXPECT_FALSE(s2.vertical());
}

TEST(segment, y_at_when_horizontal) {
  segment s{{0, 2}, {5, 2}};
  EXPECT_DOUBLE_EQ(s.y_at(0), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(3), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(5), 2.0);
}

TEST(segment, y_at_when_45_degree) {
  segment s{{0, 0}, {4, 4}};
  EXPECT_DOUBLE_EQ(s.y_at(0), 0.0);
  EXPECT_DOUBLE_EQ(s.y_at(2), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(4), 4.0);
}

TEST(segment, y_at_generic) {
  segment s{{2, 1}, {2, 5}};
  EXPECT_DOUBLE_EQ(s.y_at(2), 1.0);
  EXPECT_DOUBLE_EQ(s.y_at(10), 1.0);
}

TEST(segment, urpoint_at) {
  segment s{{0, 0}, {4, 4}};
  urpoint p = s.urpoint_at(3);
  EXPECT_DOUBLE_EQ(p.x, 3.0);
  EXPECT_DOUBLE_EQ(p.y, 3.0);
}

TEST(segment_intersection, intersect_at_center) {
  segment s1{{0, 0}, {4, 4}};
  segment s2{{0, 4}, {4, 0}};
  auto p = s1.intersection(s2);
  ASSERT_TRUE(p.has_value());
  EXPECT_DOUBLE_EQ(p->x, 2.0);
  EXPECT_DOUBLE_EQ(p->y, 2.0);
}

TEST(segment_intersection, no_intersection_parallel) {
  segment s1{{0, 0}, {4, 0}};
  segment s2{{0, 1}, {4, 1}};
  EXPECT_EQ(s1.intersection(s2), std::nullopt);
}

TEST(segment_intersection, no_intersection_collinear_disjoint) {
  segment s1{{0, 0}, {2, 0}};
  segment s2{{3, 0}, {5, 0}};
  EXPECT_EQ(s1.intersection(s2), std::nullopt);
}

TEST(segment_intersection, touch_at_endpoint) {
  segment s1{{0, 0}, {2, 2}};
  segment s2{{2, 2}, {4, 0}};
  auto p = s1.intersection(s2);
  ASSERT_TRUE(p.has_value());
  EXPECT_DOUBLE_EQ(p->x, 2.0);
  EXPECT_DOUBLE_EQ(p->y, 2.0);
}

TEST(segment_intersection, intersect_inside_bounds) {
  segment s1{{0, 0}, {4, 0}};
  segment s2{{2, -1}, {2, 1}};
  auto p = s1.intersection(s2);
  ASSERT_TRUE(p.has_value());
  EXPECT_DOUBLE_EQ(p->x, 2.0);
  EXPECT_DOUBLE_EQ(p->y, 0.0);
}

TEST(segment_intersection, extend_lines_would_intersect_but_segments_do_not) {
  segment s1{{0, 0}, {1, 1}};
  segment s2{{2, 0}, {3, 1}};
  EXPECT_EQ(s1.intersection(s2), std::nullopt);
}

} // namespace segment_tests

#include <gtest/gtest.h>

#include "snaro/segment"

using namespace snaro;

TEST(segment, is_vertical) {
  segment s1{{0,0}, {0,5}};
  EXPECT_TRUE(s1.vertical());

  segment s2{{0,0}, {5,0}};
  EXPECT_FALSE(s2.vertical());
}

TEST(segment, y_at_when_horizontal) {
  segment s{{0,2}, {5,2}};
  EXPECT_DOUBLE_EQ(s.y_at(0), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(3), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(5), 2.0);
}

TEST(segment, y_at_when_45_degree) {
  segment s{{0,0}, {4,4}};
  EXPECT_DOUBLE_EQ(s.y_at(0), 0.0);
  EXPECT_DOUBLE_EQ(s.y_at(2), 2.0);
  EXPECT_DOUBLE_EQ(s.y_at(4), 4.0);
}

TEST(segment, y_at_generic) {
  segment s{{2,1}, {2,5}};
  EXPECT_DOUBLE_EQ(s.y_at(2), 1.0);
  EXPECT_DOUBLE_EQ(s.y_at(10), 1.0);
}

TEST(segment, urpoint_at) {
  segment s{{0,0}, {4,4}};
  urpoint p = s.urpoint_at(3);
  EXPECT_DOUBLE_EQ(p.x, 3.0);
  EXPECT_DOUBLE_EQ(p.y, 3.0);
}
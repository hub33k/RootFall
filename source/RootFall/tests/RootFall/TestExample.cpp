#include <gtest/gtest.h>

// https://google.github.io/googletest/quickstart-cmake.html

// TODO (hub33k): setup proper tests config - same as RootFall
// https://www.youtube.com/watch?v=AmliviVGX8Q

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

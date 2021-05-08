#include <gtest/gtest.h>

#include <sstream>

#include "Color.h"

TEST(ColorTests, Creation) {
  Color def;
  Color zero{0, 0, 0};
  EXPECT_EQ(def, zero);

  Color c{-356, 1230, 1700};
  Color res{0, 255, 255};
  EXPECT_EQ(c, res);
}

TEST(ColorTests, Truncation) {
  {
    Color tr{0, 0, 0};
    Color g{23, 4, 51};
    g.TruncByColor(tr);
    EXPECT_EQ(tr, g);
  }
  {
    Color tr{10, 2, 10};
    Color g{23, 4, 51};
    g.TruncByColor(tr);
    EXPECT_EQ(tr, g);
  }
  {
    Color tr{100, 100, 100};
    Color g{23, 4, 51};
    g.TruncByColor(tr);
    EXPECT_EQ(g.red_, 23);
    EXPECT_EQ(g.green_, 4);
    EXPECT_EQ(g.blue_, 51);
  }
}

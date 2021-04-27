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

TEST(ColorTests, Writing) {
  Color tst{1, 2, 3};
  std::stringstream exp;
  exp << "1 2 3";
  std::stringstream res;
  res << tst;
  EXPECT_EQ(exp.str(), res.str());
}

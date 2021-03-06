#include <gtest/gtest.h>

#include "GeoVec.h"

TEST(GeoVecTests, Creation) {
  GeoVec x0{1, 2, 3};
  GeoVec x1{13, 5, -29};
  GeoVec res{x0, x1};
  EXPECT_EQ(res.x_, 12);
  EXPECT_EQ(res.y_, 3);
  EXPECT_EQ(res.z_, -32);
}

TEST(GeoVecTests, Length) {
  GeoVec x0{0, 0, 0};
  EXPECT_EQ(x0.Len(), 0);

  GeoVec x1{0, -4, 3};
  EXPECT_EQ(x1.Len(), 5);

  GeoVec x2{-4, 2, -3};
  EXPECT_EQ(x2.Len(), std::sqrt(29));
}

TEST(GeoVecTests, Norm) {
  GeoVec x0{0, -4, 3};
  x0.Norm();

  EXPECT_DOUBLE_EQ(x0.Len(), 1);
  EXPECT_DOUBLE_EQ(x0.x_, 0);
  EXPECT_DOUBLE_EQ(x0.y_, -0.8);
  EXPECT_DOUBLE_EQ(x0.z_, 0.6);

#ifndef NDEBUG
  GeoVec x1{0, 0, 0};
  EXPECT_DEATH(x1.Norm(), "");
#endif  // NDEBUG
}

TEST(GeoVecTests, DotProduct) {
  GeoVec l{1, 2, 3};
  GeoVec r{-1, -2, -3};

  EXPECT_EQ(l.Dot(r), -14);
  EXPECT_EQ(l.Dot(r), r.Dot(l));

  GeoVec x0{1, 0, 0};
  GeoVec x1{0, -39.0, 0};
  GeoVec x2{0, 0, M_PI};
  GeoVec zero{0, 0, 0};

  EXPECT_EQ(x0.Dot(x1), 0);
  EXPECT_EQ(x1.Dot(x2), 0);
  EXPECT_EQ(x2.Dot(x0), 0);

  EXPECT_EQ(zero.Dot(l), 0);

  GeoVec d1{12.45, 4.2, -5.9};
  EXPECT_EQ(d1.Dot(d1), d1.Len() * d1.Len());
}

TEST(GeoVecTests, Cross) {
  GeoVec d1{1, 2, 3};
  GeoVec d2{5, 8, -9};
  GeoVec res{-42, 24, -2};
  EXPECT_EQ(d1.Cross(d2), res);

  GeoVec x{1, 0, 0};
  GeoVec y{0, 1, 0};
  GeoVec z{0, 0, 1};

  EXPECT_EQ(x.Cross(y), z);
  EXPECT_EQ(y.Cross(x), -z);
  EXPECT_EQ(z.Cross(x), y);
  EXPECT_EQ(y.Cross(z), x);

  GeoVec p1{1, 2, M_PI};
  GeoVec p2{0.8, 1.6, 0.8 * M_PI};
  GeoVec zero{0, 0, 0};
  EXPECT_EQ(p1.Cross(p2), zero);
  EXPECT_EQ(p1.Cross(zero), zero);
}

TEST(GeoVecTests, Operators) {
  GeoVec x0{1, -2, M_PI};
  GeoVec x1{6.0, 9.0, -3 * M_PI};

  GeoVec res1{2.0, 3.0, -M_PI};
  EXPECT_EQ(x1 / 3, res1);

#ifndef NDEBUG
  EXPECT_DEATH(x1 / 0, "");
#endif  // NDEBUG
  GeoVec res2{12.0, 18.0, -6 * M_PI};
  EXPECT_EQ(res2, x1 * 2);
  EXPECT_EQ(res2, 2 * x1);

  GeoVec res3{7.0, 7.0, -2 * M_PI};
  EXPECT_EQ(res3, x1 + x0);
  EXPECT_EQ(res3, x0 + x1);

  GeoVec res4{5.0, 11.0, -4 * M_PI};
  EXPECT_EQ(res4, x1 - x0);
  EXPECT_EQ(res4, -x0 + x1);
}

TEST(GeoVecTests, GetDistance) {
  GeoVec p1{0, 1, 2};
  GeoVec p2{3, 4, 5};

  EXPECT_EQ(dist_btw_points(p1, p1), 0);
  EXPECT_EQ(dist_btw_points(p1, p2), std::sqrt(27));
  EXPECT_EQ(dist_btw_points(p2, p1), std::sqrt(27));
}

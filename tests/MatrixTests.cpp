#include <gtest/gtest.h>

#include "Matrix.h"

TEST(MatrixTests, Det2x2) {
  EXPECT_DOUBLE_EQ(Det2x2(1, 0, 0, 1), 1);
  EXPECT_DOUBLE_EQ(Det2x2(0, 1, 1, 0), -1);
  EXPECT_DOUBLE_EQ(Det2x2(1, 6, 4, 0), -24);
}

TEST(MatrixTests, Det3x3) {
  EXPECT_DOUBLE_EQ(Det3x3(Matrix3x3{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}), 1);
  EXPECT_DOUBLE_EQ(Det3x3(Matrix3x3{{0, 0, 1}, {0, 1, 0}, {1, 0, 0}}), -1);
  EXPECT_DOUBLE_EQ(Det3x3(Matrix3x3{{1, 3, 1}, {0, 2, 2}, {-2, 1, -2}}), -14);
}

TEST(MatrixTests, GetTranspose) {
  Matrix3x3 m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  Matrix3x3 mtr = GetTranspose(m);

  EXPECT_EQ(mtr.c0.x_, m.c0.x_);
  EXPECT_EQ(mtr.c1.x_, m.c0.y_);
  EXPECT_EQ(mtr.c2.x_, m.c0.z_);

  EXPECT_EQ(mtr.c0.y_, m.c1.x_);
  EXPECT_EQ(mtr.c1.y_, m.c1.y_);
  EXPECT_EQ(mtr.c2.y_, m.c1.z_);

  EXPECT_EQ(mtr.c0.z_, m.c2.x_);
  EXPECT_EQ(mtr.c1.z_, m.c2.y_);
  EXPECT_EQ(mtr.c2.z_, m.c2.z_);
}

TEST(MatrixTests, GetReverse3x3) {
  {
    Matrix3x3 m{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix3x3 mg = GetReverse3x3(m);
    EXPECT_DOUBLE_EQ(mg.c0.x_, m.c0.x_);
    EXPECT_DOUBLE_EQ(mg.c0.y_, m.c0.y_);
    EXPECT_DOUBLE_EQ(mg.c0.z_, m.c0.z_);

    EXPECT_DOUBLE_EQ(mg.c1.x_, m.c1.x_);
    EXPECT_DOUBLE_EQ(mg.c1.y_, m.c1.y_);
    EXPECT_DOUBLE_EQ(mg.c1.z_, m.c1.z_);

    EXPECT_DOUBLE_EQ(mg.c2.x_, m.c2.x_);
    EXPECT_DOUBLE_EQ(mg.c2.y_, m.c2.y_);
    EXPECT_DOUBLE_EQ(mg.c2.z_, m.c2.z_);
  }
  {
    Matrix3x3 m{{1, 0, 5}, {2, 1, 6}, {3, 4, 0}};
    Matrix3x3 mg = GetReverse3x3(m);
    Matrix3x3 exp{{-24, 20, -5}, {18, -15, 4}, {5, -4, 1}};
    EXPECT_DOUBLE_EQ(mg.c0.x_, exp.c0.x_);
    EXPECT_DOUBLE_EQ(mg.c0.y_, exp.c0.y_);
    EXPECT_DOUBLE_EQ(mg.c0.z_, exp.c0.z_);

    EXPECT_DOUBLE_EQ(mg.c1.x_, exp.c1.x_);
    EXPECT_DOUBLE_EQ(mg.c1.y_, exp.c1.y_);
    EXPECT_DOUBLE_EQ(mg.c1.z_, exp.c1.z_);

    EXPECT_DOUBLE_EQ(mg.c2.x_, exp.c2.x_);
    EXPECT_DOUBLE_EQ(mg.c2.y_, exp.c2.y_);
    EXPECT_DOUBLE_EQ(mg.c2.z_, exp.c2.z_);
  }
}

TEST(MatrixTests, ApplyToVec) {
  GeoVec v1{3, 2, 0};
  GeoVec v2{1, 0, 1};
  GeoVec v3{2, -2, 1};

  Matrix3x3 rm = GetReverse3x3(Matrix3x3{v1, v2, v3});

  GeoVec res1 = ApplyToVec(rm, v1);
  GeoVec exp1{1, 0, 0};
  EXPECT_DOUBLE_EQ(res1.x_, exp1.x_);
  EXPECT_DOUBLE_EQ(res1.y_, exp1.y_);
  EXPECT_DOUBLE_EQ(res1.z_, exp1.z_);

  GeoVec res2 = ApplyToVec(rm, v2);
  GeoVec exp2{0, 1, 0};
  EXPECT_DOUBLE_EQ(res2.x_, exp2.x_);
  EXPECT_DOUBLE_EQ(res2.y_, exp2.y_);
  EXPECT_DOUBLE_EQ(res2.z_, exp2.z_);

  GeoVec res3 = ApplyToVec(rm, v3);
  GeoVec exp3{0, 0, 1};
  EXPECT_DOUBLE_EQ(res3.x_, exp3.x_);
  EXPECT_DOUBLE_EQ(res3.y_, exp3.y_);
  EXPECT_DOUBLE_EQ(res3.z_, exp3.z_);
}

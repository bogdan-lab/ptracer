#include <gtest/gtest.h>

#include <cmath>

#include "Objects.h"
#include "Ray.h"
#include "Reflector.h"

TEST(SphereTests, GetNorm) {
  Sphere s{GeoVec{0, 0, 0}, M_PI};
  {
    GeoVec p{M_PI, 0, 0};
    GeoVec n = s.GetNorm(p);
    GeoVec exp_n{1, 0, 0};
    EXPECT_EQ(n, exp_n);
  }
  {
    GeoVec p{0, M_PI, 0};
    GeoVec n = s.GetNorm(p);
    GeoVec exp_n{0, 1, 0};
    EXPECT_EQ(n, exp_n);
  }
  {
    GeoVec p{0, 0, -M_PI};
    GeoVec n = s.GetNorm(p);
    GeoVec exp_n{0, 0, -1};
    EXPECT_EQ(n, exp_n);
  }
  Sphere s2{GeoVec{1, 2, 3}, 1};
  {
    GeoVec p{1, 2.8, 2.4};
    GeoVec n = s2.GetNorm(p);
    GeoVec exp_n{0, 0.8, -0.6};
    EXPECT_DOUBLE_EQ(n.x_, exp_n.x_);
    EXPECT_DOUBLE_EQ(n.y_, exp_n.y_);
    EXPECT_DOUBLE_EQ(n.z_, exp_n.z_);
  }
}

TEST(SphereTests, Reflect) {
  Sphere s{GeoVec{0, 0, 0}, 3};
  s.SetPolishness(1.0).SetReflectionCoef(1.0);
#ifndef NDEBUG
  {
    Ray r{GeoVec{1, 0, 0}, GeoVec{12, 5, 6}};
    EXPECT_DEATH(s.Reflect(r, 20), "");
  }
#endif  // NDEBUG
  {     // 0 degree incident
    Ray r{GeoVec{5, 0, 0}, GeoVec{-1, 0, 0}};
    EXPECT_TRUE(s.TryReflect(r, 2.0));
    GeoVec exp_pos{3, 0, 0};
    GeoVec exp_dir{1, 0, 0};
    EXPECT_EQ(exp_dir, r.GetDir());
    // EXPECT_NEAR(exp_pos.x_, r.GetPos().x_, 1.1 * s.GetHitPrecision());
    // EXPECT_NEAR(exp_pos.y_, r.GetPos().y_, s.GetHitPrecision());
    // EXPECT_NEAR(exp_pos.z_, r.GetPos().z_, s.GetHitPrecision());
    EXPECT_DOUBLE_EQ(exp_pos.x_, r.GetPos().x_);
    EXPECT_DOUBLE_EQ(exp_pos.y_, r.GetPos().y_);
    EXPECT_DOUBLE_EQ(exp_pos.z_, r.GetPos().z_);
  }

  Sphere s2{GeoVec{0, 0, 0}, 5};
  s2.SetPolishness(1.0);
  {  // general case
    Ray r{GeoVec{7, 3, 0}, GeoVec{-1, 0, 0}};
    EXPECT_TRUE(s2.TryReflect(r, 3));
    GeoVec exp_pos{4, 3, 0};
    GeoVec exp_dir{0.28, 0.96, 0};
    EXPECT_DOUBLE_EQ(exp_dir.x_, r.GetDir().x_);
    EXPECT_DOUBLE_EQ(exp_dir.y_, r.GetDir().y_);
    EXPECT_DOUBLE_EQ(exp_dir.z_, r.GetDir().z_);
    EXPECT_DOUBLE_EQ(exp_pos.x_, r.GetPos().x_);
    EXPECT_DOUBLE_EQ(exp_pos.y_, r.GetPos().y_);
    EXPECT_DOUBLE_EQ(exp_pos.z_, r.GetPos().z_);
  }
}

TEST(SphereTests, GetClosestDistance) {
  Sphere s{GeoVec{0, 0, 0}, 5};
#ifndef NDEBUG
  {
    Ray r{GeoVec{5, 0, 0}, GeoVec{1, 2, 3}};
    EXPECT_DEATH(s.GetClosesDist(r), "");
  }
#endif  // NDEBUG
  {     // trivial distance
    Ray r{GeoVec{7, 0, 0}, GeoVec{-1, 0, 0}};
    std::optional<double> res = s.GetClosesDist(r);
    EXPECT_TRUE(res);
    EXPECT_EQ(2, *res);
  }
  {  // wrong direction
    Ray r{GeoVec{7, 0, 0}, GeoVec{1, 0, 0}};
    std::optional<double> res = s.GetClosesDist(r);
    EXPECT_FALSE(res);
  }
  {  // correct direction but missed
    Ray r{GeoVec{7, 0, 0}, GeoVec{-1, 3, 0}};
    std::optional<double> res = s.GetClosesDist(r);
    EXPECT_FALSE(res);
  }
  {  // not direct hit
    Ray r{GeoVec{7, 0, 0}, GeoVec{-3, 3, 0}};
    std::optional<double> res = s.GetClosesDist(r);
    EXPECT_TRUE(res);
    EXPECT_DOUBLE_EQ(std::sqrt(18), *res);
  }
  {  // grazing
    Ray r{GeoVec{7, 5, 0}, GeoVec{-1, 0, 0}};
    std::optional<double> res = s.GetClosesDist(r);
    EXPECT_FALSE(res);
  }
}

TEST(TriangleTests, Creation) {
  {
    Triangle tr{GeoVec{0, 0, 0}, GeoVec{0, 17, 0}, GeoVec{1.3, 0, 0}};
    GeoVec exp_n{0, 0, -1};
    GeoVec res_n = tr.GetNorm(GeoVec{0, 0, 0});
    EXPECT_EQ(exp_n, res_n);
  }
  {
    Triangle tr{GeoVec{0, 0, 0}, GeoVec{0, 0, 0.1}, GeoVec{10, 0, 0}};
    GeoVec exp_n{0, 1, 0};
    GeoVec res_n = tr.GetNorm(GeoVec{0, 0, 0});
    EXPECT_EQ(exp_n, res_n);
  }
  {
    Triangle tr{GeoVec{0, 0, 0}, GeoVec{0, M_PI, 0}, GeoVec{0, 0, -1.3}};
    GeoVec exp_n{-1, 0, 0};
    GeoVec res_n = tr.GetNorm(GeoVec{0, 0, 0});
    EXPECT_EQ(exp_n, res_n);
  }
}

TEST(TriangleTests, CheckPointInTriangle) {
  {  // in plane z = 0
    Triangle tr{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
    EXPECT_TRUE(tr.CheckInTriangle({0.25, 0.25, 0}));
    EXPECT_FALSE(tr.CheckInTriangle({1.5, 0.25, 0}));
    EXPECT_FALSE(tr.CheckInTriangle({0.25, 1.5, 0}));
    EXPECT_FALSE(tr.CheckInTriangle({-0.25, 0.25, 0}));
    EXPECT_FALSE(tr.CheckInTriangle({0.25, -0.25, 0}));
  }
  {  // in plane x = 10
    Triangle tr{{10, 0, 0}, {10, 1, 0}, {10, 0, 1}};
    EXPECT_TRUE(tr.CheckInTriangle({10, 0.25, 0.25}));
    EXPECT_FALSE(tr.CheckInTriangle({10, 1.25, 0.25}));
    EXPECT_FALSE(tr.CheckInTriangle({10, 1.25, -0.25}));
    EXPECT_FALSE(tr.CheckInTriangle({10, -1.25, -1.25}));
    EXPECT_FALSE(tr.CheckInTriangle({10, 1.25, 1.25}));
  }
  {  // in plane not parallel to standart plane
    Triangle tr{{0, 0, 0}, {1, 1, -1}, {0, 0, 1}};
    EXPECT_TRUE(tr.CheckInTriangle({0.25, 0.25, 0.25}));
    EXPECT_FALSE(tr.CheckInTriangle({0.25, 0.25, -0.7}));
    EXPECT_FALSE(tr.CheckInTriangle({0.25, 0.25, 0.7}));
    EXPECT_FALSE(tr.CheckInTriangle({-0.25, -0.25, 0.0}));
  }
}

TEST(TriangleTests, GetClosestDistance) {
  Triangle tr{GeoVec{0, 0, 0}, GeoVec{1, 0, 0}, GeoVec{0, 1, 0}};
  {
    GeoVec pos{0.0, 0.0, 3};
    GeoVec dir{0.0, 4, -3};
    Ray r{pos, dir};
    std::optional<double> dist = tr.GetClosesDist(r);
    EXPECT_FALSE(dist);
  }
  {
    GeoVec pos{0.0, 0.0, 3};
    GeoVec dir{0.0, 4, 3};
    Ray r{pos, dir};
    std::optional<double> dist = tr.GetClosesDist(r);
    EXPECT_FALSE(dist);
  }
  {
    GeoVec pos{0.25, 0.25, 37};
    GeoVec dir{0, 0, -1};
    Ray r{pos, dir};
    std::optional<double> dist = tr.GetClosesDist(r);
    EXPECT_TRUE(dist);
    EXPECT_DOUBLE_EQ(*dist, 37);
  }
  {
    GeoVec pos{0.25, -3.75, 3};
    GeoVec dir{0, 4, -3};
    Ray r{pos, dir};
    std::optional<double> dist = tr.GetClosesDist(r);
    EXPECT_TRUE(dist);
    EXPECT_DOUBLE_EQ(*dist, 5);
  }
  Triangle tr2{GeoVec{0, 0, 0}, GeoVec{0, 1, 0}, GeoVec{1, 0, 0}};
  {
    GeoVec pos{0.25, 0.25, 37};
    GeoVec dir{0, 0, -1};
    Ray r{pos, dir};
    std::optional<double> dist = tr2.GetClosesDist(r);
    EXPECT_FALSE(dist);
  }
}

TEST(TriangleTests, Reflect) {
  Triangle tr{GeoVec{0, 0, 0}, GeoVec{1, 0, 0}, GeoVec{0, 1, 0}};
  tr.SetPolishness(1.0).SetReflectionCoef(1.0);
#ifndef NDEBUG
  {  // incorrect data
    GeoVec pos{0.25, 0.25, -37};
    GeoVec dir{0.0, 0.0, 1};
    Ray r{pos, dir};
    EXPECT_DEATH(tr.Reflect(r, 37), "");
  }
#endif  // NDEBUG
  {     // Ray 0 degree
    GeoVec pos{0.25, 0.25, 37};
    GeoVec dir{0.0, 0.0, -1};
    Ray r{pos, dir};
    EXPECT_TRUE(tr.TryReflect(r, 37));
    GeoVec exp_dir{0.0, 0.0, 1.0};
    GeoVec exp_pos{0.25, 0.25, 0.0};
    EXPECT_EQ(exp_dir, r.GetDir());
    EXPECT_DOUBLE_EQ(exp_pos.x_, r.GetPos().x_);
    EXPECT_DOUBLE_EQ(exp_pos.y_, r.GetPos().y_);
    EXPECT_DOUBLE_EQ(exp_pos.z_, r.GetPos().z_);
  }
  {  // General case
    GeoVec pos{0.25, -3.75, 3};
    GeoVec dir{0.0, 4.0, -3};
    Ray r{pos, dir};
    EXPECT_TRUE(tr.TryReflect(r, 5));
    GeoVec exp_dir{0.0, 4.0, 3.0};
    exp_dir.Norm();
    // For some reasons z component fails double comparison
    GeoVec exp_pos{0.25, 0.25, 0.0};
    EXPECT_EQ(exp_dir, r.GetDir());
    EXPECT_DOUBLE_EQ(exp_pos.x_, r.GetPos().x_);
    EXPECT_DOUBLE_EQ(exp_pos.y_, r.GetPos().y_);
    EXPECT_NEAR(exp_pos.z_, r.GetPos().z_, 1e-15);
  }
}

TEST(ReflectorTests, DiffuseReflector) {
  std::mt19937 rnd(42);
  GeoVec dir{0, 0, -1};
  size_t check_num = 1000;
  {
    GeoVec norm{0, 0, 1};
    for (size_t i = 0; i < check_num; i++) {
      GeoVec vec = DiffuseReflect(rnd, dir, norm);
      EXPECT_GE(vec.Dot(norm), 0);
    }
  }
  {
    GeoVec norm{1, 2, 3};
    for (size_t i = 0; i < check_num; i++) {
      GeoVec vec = DiffuseReflect(rnd, dir, norm);
      EXPECT_GE(vec.Dot(norm), 0);
    }
  }
}

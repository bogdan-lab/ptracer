#include <gtest/gtest.h>
#include <cmath>
#include "Objects.h"
#include "Ray.h"

TEST (SphereTests, GetNorm) {
    Sphere s{GeoVec{0,0,0}, M_PI};
    {
        GeoVec p{M_PI,0,0};
        GeoVec n = s.GetNorm(p);
        GeoVec exp_n{1,0,0};
        EXPECT_EQ(n, exp_n);
    }
    {
        GeoVec p{0,M_PI,0};
        GeoVec n = s.GetNorm(p);
        GeoVec exp_n{0,1,0};
        EXPECT_EQ(n, exp_n);
    }
    {
        GeoVec p{0,0,-M_PI};
        GeoVec n = s.GetNorm(p);
        GeoVec exp_n{0,0,-1};
        EXPECT_EQ(n, exp_n);
    }
    Sphere s2{GeoVec{1,2,3}, 1};
    {
        GeoVec p{1,2.8,2.4};
        GeoVec n = s2.GetNorm(p);
        GeoVec exp_n{0, 0.8, -0.6};
        EXPECT_DOUBLE_EQ(n.x_, exp_n.x_);
        EXPECT_DOUBLE_EQ(n.y_, exp_n.y_);
        EXPECT_DOUBLE_EQ(n.z_, exp_n.z_);
    }
}


TEST (SphereTests, Reflect) {
    Sphere s{GeoVec{0,0,0}, 3};
    Ray r_inside{GeoVec{1,0,0}, GeoVec{12,5,6}};
    EXPECT_DEATH(s.Reflect(r_inside, 20), "");

    {//0 degree incident
        Ray r{GeoVec{5,0,0}, GeoVec{-1,0,0}};
        s.Reflect(r, 2.0);
        GeoVec exp_pos{3,0,0};
        GeoVec exp_dir{1,0,0};
        EXPECT_EQ(exp_dir, r.GetDir());
        EXPECT_NEAR(exp_pos.x_, r.GetPos().x_, 1.1*s.GetHitPrecision());
        EXPECT_NEAR(exp_pos.y_, r.GetPos().y_, s.GetHitPrecision());
        EXPECT_NEAR(exp_pos.z_, r.GetPos().z_, s.GetHitPrecision());
    }

    Sphere s2{GeoVec{0,0,0}, 5};
    {//general case
        Ray r{GeoVec{7,3,0}, GeoVec{-1,0,0}};
        s2.Reflect(r, 3);
        GeoVec exp_pos{4,3,0};
        GeoVec exp_dir{0.28, 0.96, 0};
        EXPECT_NEAR(exp_dir.x_, r.GetDir().x_, s2.GetHitPrecision());
        EXPECT_NEAR(exp_dir.y_, r.GetDir().y_, s2.GetHitPrecision());
        EXPECT_NEAR(exp_dir.z_, r.GetDir().z_, s2.GetHitPrecision());
        EXPECT_NEAR(exp_pos.x_, r.GetPos().x_, 1.1*s2.GetHitPrecision());
        EXPECT_NEAR(exp_pos.y_, r.GetPos().y_, 1.1*s2.GetHitPrecision());
        EXPECT_NEAR(exp_pos.z_, r.GetPos().z_, 1.1*s2.GetHitPrecision());
    }
}


TEST (SphereTests, GetClosestDistance) {
    Sphere s{GeoVec{0,0,0}, 5};
    Ray death_ray{GeoVec{5,0,0}, GeoVec{1,2,3}};
    EXPECT_DEBUG_DEATH(s.GetClosesDist(death_ray),"");
    {	//trivial distance
        Ray r{GeoVec{7,0,0}, GeoVec{-1,0,0}};
        std::optional<double> res = s.GetClosesDist(r);
        EXPECT_TRUE(res);
        EXPECT_EQ(2, *res);
    }
    {	//wrong direction
        Ray r{GeoVec{7,0,0}, GeoVec{1,0,0}};
        std::optional<double> res = s.GetClosesDist(r);
        EXPECT_FALSE(res);
    }
    {	//correct direction but missed
        Ray r{GeoVec{7,0,0}, GeoVec{-1,3,0}};
        std::optional<double> res = s.GetClosesDist(r);
        EXPECT_FALSE(res);
    }
    {	//not direct hit
        Ray r{GeoVec{7,0,0}, GeoVec{-3,3,0}};
        std::optional<double> res = s.GetClosesDist(r);
        EXPECT_TRUE(res);
        EXPECT_DOUBLE_EQ(std::sqrt(18), *res);
    }
    {	//grazing
        Ray r{GeoVec{7,5,0}, GeoVec{-1,0,0}};
        std::optional<double> res = s.GetClosesDist(r);
        EXPECT_FALSE(res);
    }
}


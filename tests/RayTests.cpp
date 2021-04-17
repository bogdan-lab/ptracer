#include <gtest/gtest.h>
#include "Ray.h"


TEST(RayTests, Creation) {
    Ray r{GeoVec{1,2,3}, GeoVec{0, 4, 3}};

    GeoVec exp_dir{0.0,0.8,0.6};
    GeoVec exp_pos{1,2,3};

    EXPECT_EQ(r.GetDir(), exp_dir);
    EXPECT_EQ(r.GetPos(), exp_pos);
}


TEST (RayTests, Advance) {
    {
        Ray r{GeoVec{1,2,3}, GeoVec{0, 0, 1}};
        r.Advance(3);
        GeoVec exp_pos{1,2,6};
        GeoVec exp_dir{0,0,1};
        EXPECT_EQ(r.GetPos(), exp_pos);
        EXPECT_EQ(r.GetDir(), exp_dir);
#ifndef NDEBUG
        EXPECT_DEATH(r.Advance(-7), "");
#endif //NDEBUG
    }
    {
        Ray r{GeoVec{1,2,6}, GeoVec{0,0,1}};
        r.StepBack(7);
        GeoVec exp_pos{1,2,-1};
        GeoVec exp_dir{0,0,1};
        EXPECT_EQ(r.GetPos(), exp_pos);
        EXPECT_EQ(r.GetDir(), exp_dir);
    }
    {
        Ray r{GeoVec{1,2,3}, GeoVec{0,4,3}};
        r.Advance(10);
        GeoVec exp_pos{1,10,9};
        GeoVec exp_dir{0,0.8,0.6};
        EXPECT_EQ(r.GetPos(), exp_pos);
        EXPECT_EQ(r.GetDir(), exp_dir);
    }
}


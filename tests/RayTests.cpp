#include <gtest/gtest.h>
#include "Ray.h"


TEST(RayTests, Creation) {
    Ray r{GeoVec{1,2,3}, GeoVec{0, 4, 3}};

    GeoVec exp_dir{0.0,0.8,0.6};
    GeoVec exp_pos{1,2,3};

    EXPECT_EQ(r.dir_, exp_dir);
    EXPECT_EQ(r.pos_, exp_pos);
}


TEST (RayTests, Advance) {
    Ray r{GeoVec{1,2,3}, GeoVec{0, 0, 1}};
    r.Advance(3);
    GeoVec exp_pos{1,2,6};
    GeoVec exp_dir{0,0,1};
    EXPECT_EQ(r.pos_, exp_pos);
    EXPECT_EQ(r.dir_, exp_dir);

    r.Advance(-7);
    GeoVec exp_pos2{1,2,-1};
    GeoVec exp_dir2{0,0,1};
    EXPECT_EQ(r.pos_, exp_pos2);
    EXPECT_EQ(r.dir_, exp_dir2);

    Ray r2{GeoVec{1,2,3}, GeoVec{0,4,3}};
    r2.Advance(10);
    GeoVec exp_pos3{1,10,9};
    GeoVec exp_dir3{0,0.8,0.6};
    EXPECT_EQ(r2.pos_, exp_pos3);
    EXPECT_EQ(r2.dir_, exp_dir3);
}


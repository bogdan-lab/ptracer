#include <gtest/gtest.h>

#include <cmath>
#include <random>

#include "Pixel.h"

TEST(PixelTests, CreateRays) {
  // Check that all created rays pathes through the given tile
  pixel::Tile tile{GeoVec(0.0, 1.0, 1.0), GeoVec(0.1, 0.0, 0.0),
                   GeoVec(0.0, 0.0, -0.1)};
  std::mt19937 rnd(42);
  GeoVec ray_start{0.0, 0.0, 0.0};
  std::vector<Ray> res_rays =
      pixel::CreateRays(tile, ray_start, rnd, /*ray_num=*/10000);
  // move all rays to the screen position and check
  auto move_ray_to_screen = [&](Ray ray, double precision) {
    double rhs_d = 3 * GeoVec(ray_start, tile.top_left).Len();
    int num = static_cast<int>(std::log2(rhs_d / precision)) + 1;
    double lhs_d = 0.0;
    while (num--) {
      double curr_d = (rhs_d + lhs_d) / 2;
      GeoVec pos = ray.GetPos() + curr_d * ray.GetDir();
      if (pos.y_ > 1.0) {
        rhs_d = curr_d;
      } else {
        lhs_d = curr_d;
      }
    }
    ray.Advance(0.5 * (lhs_d + rhs_d));
    return ray;
  };

  double precision = 1e-15;
  for (const auto& ray : res_rays) {
    Ray moved = move_ray_to_screen(ray, precision);
    EXPECT_NEAR(moved.GetPos().y_, 1.0, precision);  // in screen plane
    EXPECT_GE(moved.GetPos().x_, 0.0);
    EXPECT_LE(moved.GetPos().x_, 0.1);
    EXPECT_LE(moved.GetPos().z_, 1.0);
    EXPECT_GE(moved.GetPos().z_, 0.9);
  }
}

TEST(PixelTests, CreateTiles) {
  // Creating tiles for simple screen and check that they are in corect order
  std::vector<pixel::Tile> tiles = pixel::CreateTiles(
      /*top_left=*/{0.0, 10.0, 0.0}, /*top_right=*/{10.0, 10.0, 0.0},
      /*bot_left=*/{0.0, 0.0, 0.0}, /*width=*/2, /*height=*/4);
  ASSERT_EQ(tiles.size(), 8);

  GeoVec expected_width_vec{5.0, 0.0, 0.0};
  GeoVec expected_height_vec{0.0, -2.5, 0.0};

  EXPECT_EQ(tiles[0].top_left, GeoVec(0.0, 10.0, 0.0));
  EXPECT_EQ(tiles[0].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[0].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[1].top_left, GeoVec(5.0, 10.0, 0.0));
  EXPECT_EQ(tiles[1].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[1].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[2].top_left, GeoVec(0.0, 7.5, 0.0));
  EXPECT_EQ(tiles[2].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[2].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[3].top_left, GeoVec(5.0, 7.5, 0.0));
  EXPECT_EQ(tiles[3].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[3].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[4].top_left, GeoVec(0.0, 5.0, 0.0));
  EXPECT_EQ(tiles[4].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[4].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[5].top_left, GeoVec(5.0, 5.0, 0.0));
  EXPECT_EQ(tiles[5].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[5].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[6].top_left, GeoVec(0.0, 2.5, 0.0));
  EXPECT_EQ(tiles[6].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[6].height_vec, expected_height_vec);

  EXPECT_EQ(tiles[7].top_left, GeoVec(5.0, 2.5, 0.0));
  EXPECT_EQ(tiles[7].width_vec, expected_width_vec);
  EXPECT_EQ(tiles[7].height_vec, expected_height_vec);
}

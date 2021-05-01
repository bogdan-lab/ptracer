#include <Camera.h>
#include <gtest/gtest.h>

TEST(CameraTests, Creation) {
#ifndef NDEBUG
  {
    Camera cam;
    cam.SetLeftCoor(100).SetRightCoor(0);
    EXPECT_DEATH(cam.CreatePixel(0), "");
  }
  {
    Camera cam;
    cam.SetBotCoor(0).SetTopCoor(100);
    EXPECT_DEATH(cam.CreatePixel(0), "");
  }
  {
    Camera cam;
    cam.SetWidthInPixel(0);
    EXPECT_DEATH(cam.CreatePixel(0), "");
  }
  {
    Camera cam;
    cam.SetHeightInPixel(0);
    EXPECT_DEATH(cam.CreatePixel(0), "");
  }
  {
    Camera cam;
    EXPECT_DEATH(cam.CreatePixel(std::numeric_limits<size_t>::max()), "");
  }
  {
    Camera cam;
    EXPECT_DEATH(Camera::SetSamplePerPixel(0), "");
  }
#endif
  {
    Camera cam;
    cam.SetSamplePerPixel(23);
    auto res = cam.GetPixelRays(0);
    EXPECT_EQ(res.size(), 23);
  }
}

TEST(CameraTests, CreatePixel) {
  Camera cam;
  cam.SetSamplePerPixel(1000);
  cam.SetWidthInPixel(1).SetHeightInPixel(1);
  double dx = (cam.GetRightCoor() - cam.GetLeftCoor()) /
              static_cast<double>(cam.GetWidthInPx());
  double dy = (cam.GetBotCoor() - cam.GetTopCoor()) /
              static_cast<double>(cam.GetHeightInPx());
  double px_radius = 0.5 * std::sqrt(dx * dx + dy * dy);
  const auto& d = cam.GetDistToPlane();
  double min_cos_alpha = d / std::sqrt(d * d + px_radius * px_radius);
  auto px_r = cam.GetPixelRays(0);
  GeoVec cam_dir{0, 0, 1};
  for (const auto& r : px_r) {
    double cos_alpha = cam_dir.Dot(r.GetDir());
    EXPECT_GE(cos_alpha, min_cos_alpha);
  }
}

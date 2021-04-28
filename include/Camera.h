#ifndef CAMERA_H
#define CAMERA_H

#include <cstdlib>
#include <optional>
#include <random>
#include <vector>

#include "GeoVec.h"
#include "Pixel.h"
#include "Ray.h"

class Camera {
  // Top left point is (0,0)
 private:
  // input fields
  static size_t SAMPLES_PER_PIXEL;
  double left_coor_{0.0};
  double right_coor_{600.0};
  double top_coor_{0.0};
  double bot_coor_{400.0};
  size_t w_px_num_{600};
  size_t h_px_num_{400};
  double dist_to_plane_{500};

  // inner fields
  GeoVec pos_;  // Camera position
  double dx_;   // horizontal size of the pixel
  double dy_;   // vertical size of the pixel
  mutable std::mt19937 rnd_{0};

 public:
  static void SetSamplePerPixel(size_t g_num) { SAMPLES_PER_PIXEL = g_num; }

  Camera()
      : pos_(0.5 * (right_coor_ + left_coor_), 0.5 * (bot_coor_ + top_coor_),
             -dist_to_plane_) {
    assert(right_coor_ > left_coor_);
    assert(top_coor_ > bot_coor_);
    assert(w_px_num_ != 0);
    assert(h_px_num_ != 0);
    dx_ = (right_coor_ - left_coor_) / static_cast<double>(w_px_num_);
    dy_ = (bot_coor_ - top_coor_) / static_cast<double>(h_px_num_);
  }

  // TODO bunch of setters, since we have here a lot of double parameters

  Camera& SetSeed(size_t new_s) {
    rnd_.seed(new_s);
    return *this;
  }

  Ray MakeCameraRay(double x_coor, double y_coor) const {
    GeoVec dir = {x_coor - pos_.x_, y_coor - pos_.y_, dist_to_plane_};
    return Ray{pos_, dir};
  }

  Pixel CreatePixel(size_t idx) const {
    // idx - pixel idx on the screen. Assume that pixels are counted from the
    // top left corner to the right
    assert(SAMPLES_PER_PIXEL > 0);
    double l_bnd = left_coor_ + static_cast<double>(idx % w_px_num_) * dx_;
    double t_bnd = top_coor_ + static_cast<double>(idx / h_px_num_) * dy_;
    std::uniform_real_distribution<double> x_dist{l_bnd, l_bnd + dx_};
    std::uniform_real_distribution<double> y_dist{t_bnd, t_bnd + dy_};
    std::vector<Ray> px_rays;
    px_rays.reserve(SAMPLES_PER_PIXEL);
    for (size_t i = 0; i < SAMPLES_PER_PIXEL; i++) {
      px_rays.push_back(MakeCameraRay(x_dist(rnd_), y_dist(rnd_)));
    }
    return Pixel{std::move(px_rays)};
  }

  double GetLeftCoor() const { return left_coor_; }
  double GetRightCoor() const { return right_coor_; }
  double GetTopCoor() const { return top_coor_; }
  double GetBotCoor() const { return bot_coor_; }
  size_t GetWidthInPx() const { return w_px_num_; }
  size_t GetHeightInPx() const { return h_px_num_; }
  double GetDistToPlane() const { return dist_to_plane_; }
  const GeoVec& GetCamPos() const { return pos_; }
  double GetXPXSize() const { return dx_; }
  double GetYPXSize() const { return dy_; }
};

inline size_t Camera::SAMPLES_PER_PIXEL = 10;
#endif  // CAMERA_H

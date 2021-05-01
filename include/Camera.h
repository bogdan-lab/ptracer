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
  mutable std::mt19937 rnd_{0};

 public:
  static void SetSamplePerPixel(size_t g_num) {
    assert(g_num != 0);
    SAMPLES_PER_PIXEL = g_num;
  }

  Camera() = default;

  Camera& SetLeftCoor(double g_coor) {
    left_coor_ = g_coor;
    return *this;
  }
  Camera& SetRightCoor(double g_coor) {
    right_coor_ = g_coor;
    return *this;
  }
  Camera& SetTopCoor(double g_coor) {
    top_coor_ = g_coor;
    return *this;
  }
  Camera& SetBotCoor(double g_coor) {
    bot_coor_ = g_coor;
    return *this;
  }
  Camera& SetWidthInPixel(size_t g_num) {
    w_px_num_ = g_num;
    return *this;
  }
  Camera& SetHeightInPixel(size_t g_num) {
    h_px_num_ = g_num;
    return *this;
  }
  Camera& SetDistToPlane(double g_dist) {
    dist_to_plane_ = g_dist;
    return *this;
  }

  Camera& SetSeed(size_t new_s) {
    rnd_.seed(new_s);
    return *this;
  }

  Pixel CreatePixel(size_t idx) const {
    // idx - pixel idx on the screen. Assume that pixels are counted from the
    // top left corner to the right
    assert(idx < GetPxNum());
    assert(right_coor_ > left_coor_);
    assert(top_coor_ > bot_coor_);
    assert(w_px_num_ != 0);
    assert(h_px_num_ != 0);
    double dx = (right_coor_ - left_coor_) / static_cast<double>(w_px_num_);
    double dy = (bot_coor_ - top_coor_) / static_cast<double>(h_px_num_);
    double l_bnd = left_coor_ + static_cast<double>(idx % w_px_num_) * dx;
    double t_bnd = top_coor_ + static_cast<double>(idx / h_px_num_) * dy;
    std::uniform_real_distribution<double> x_dist{l_bnd, l_bnd + dx};
    std::uniform_real_distribution<double> y_dist{t_bnd, t_bnd + dy};
    GeoVec pos{0.5 * (right_coor_ + left_coor_), 0.5 * (top_coor_ + bot_coor_),
               -dist_to_plane_};
    std::vector<Ray> px_rays;
    px_rays.reserve(SAMPLES_PER_PIXEL);
    for (size_t i = 0; i < SAMPLES_PER_PIXEL; i++) {
      px_rays.emplace_back(pos, GeoVec{x_dist(rnd_) - pos.x_,
                                       y_dist(rnd_) - pos.y_, dist_to_plane_});
    }
    return Pixel{std::move(px_rays)};
  }

  double GetLeftCoor() const { return left_coor_; }
  double GetRightCoor() const { return right_coor_; }
  double GetTopCoor() const { return top_coor_; }
  double GetBotCoor() const { return bot_coor_; }
  size_t GetWidthInPx() const { return w_px_num_; }
  size_t GetHeightInPx() const { return h_px_num_; }
  size_t GetPxNum() const { return w_px_num_ * h_px_num_; }
  double GetDistToPlane() const { return dist_to_plane_; }
};

inline size_t Camera::SAMPLES_PER_PIXEL = 10;
#endif  // CAMERA_H

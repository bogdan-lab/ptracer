#ifndef CAMERA_H
#define CAMERA_H

#include <cstdlib>
#include <optional>
#include <random>
#include <vector>

#include "Color.h"
#include "GeoVec.h"
#include "Pixel.h"
#include "Ray.h"

// TODO maybe funny to add possibility to set camera position according to x and
// y coordinates

// TODO maybe it is batter to gather all these static general settings into some
// config class
/**
 * This class contains general information about camera and picture. This is
 * coordinates of picture edges, distance from camera to the picture plane. For
 * now camera position in x and y coordinates is laways in the center of the
 * picture. Picture coordinates are set in the way that top left corner
 * coordinates are (0, 0). Class also contains information about pixel number in
 * the picture. It also defines few general parameters, such as: empty color -
 * color which is obtained in the case when light beam does not hit any material
 * samples per pixel - number of rays emited to calculate color in one pixel
 */
class Camera {
 private:
  // input fields
  static size_t SAMPLES_PER_PIXEL;
  static Color EMPTY_COLOR;
  double left_coor_{0.0};
  double right_coor_{600.0};
  double top_coor_{0.0};
  double bot_coor_{400.0};
  uint32_t w_px_num_{600};
  uint32_t h_px_num_{400};
  double dist_to_plane_{500};

  // inner fields
  mutable std::mt19937 rnd_{0};

 public:
  /**
   * Sets number of rays which will be emitted for each pixel in order to
   * calculate its color (default 10)
   */
  static void SetSamplePerPixel(size_t g_num) {
    assert(g_num != 0);
    SAMPLES_PER_PIXEL = g_num;
  }
  /**
   * Sets background color - the one which will be assigned to pixel for which
   * all rays miss all objects (default black)
   */
  static constexpr void SetEmptyColor(Color g_c) {
    EMPTY_COLOR = std::move(g_c);
  }
  /**
   * By default picture pixel resolution is 600x400
   * background color is black
   * for each pixel 10 rays will be used to calculate its color
   * distance from camera to the picture plane 500
   * horizontal span of the picture - [0.0, 600.0]
   * vertical span of the picture - [0.0, 400.0]
   */
  constexpr Camera() = default;
  /** Set left coordinate edge of the picutre (default 0.0)*/
  constexpr Camera& SetLeftCoor(double g_coor) {
    left_coor_ = g_coor;
    return *this;
  }
  /** Set right coordinate edge of the picture (default 600.0)*/
  constexpr Camera& SetRightCoor(double g_coor) {
    right_coor_ = g_coor;
    return *this;
  }
  /** Set top coordinate edge of the picture (default 0.0)*/
  constexpr Camera& SetTopCoor(double g_coor) {
    top_coor_ = g_coor;
    return *this;
  }
  /** Set bot coordinate edge of the picture (default 400.0)*/
  constexpr Camera& SetBotCoor(double g_coor) {
    bot_coor_ = g_coor;
    return *this;
  }
  /** Set number of pixels in picture in horizontal direction (default 600)*/
  constexpr Camera& SetWidthInPixel(uint32_t g_num) {
    w_px_num_ = g_num;
    return *this;
  }
  /** Set number of pixels in picture in vertical direction (default 400)*/
  constexpr Camera& SetHeightInPixel(uint32_t g_num) {
    h_px_num_ = g_num;
    return *this;
  }
  /** Set distance from camera to the picture plane (default 500)*/
  constexpr Camera& SetDistToPlane(double g_dist) {
    dist_to_plane_ = g_dist;
    return *this;
  }
  /**
   * For generating rays camera uses random generator. This method allows to
   * provide seed for this generator
   */
  Camera& SetSeed(size_t new_s) {
    rnd_.seed(new_s);
    return *this;
  }
  /**
   * Method returns a collection of rays for the pixel with givem index.
   *
   * @param idx - pixel idx on the screen. Assume that pixels are counted from
   * the top left corner to the right
   *
   * @warning method expects pixel index to have valid value and also that all
   * parameters of picture (coordinates, pixels number) also have valid values
   *
   * @return collection of rays for given pixes which needs to be emitted n
   * order to calculate its color
   */
  std::vector<Ray> GetPixelRays(size_t idx) const {
    assert(idx < GetPxNum());
    assert(right_coor_ > left_coor_);
    assert(top_coor_ < bot_coor_);
    assert(w_px_num_ != 0);
    assert(h_px_num_ != 0);
    double dx = (right_coor_ - left_coor_) / static_cast<double>(w_px_num_);
    double dy = (bot_coor_ - top_coor_) / static_cast<double>(h_px_num_);
    double l_bnd = left_coor_ + static_cast<double>(idx % w_px_num_) * dx;
    double t_bnd = top_coor_ + static_cast<double>(idx / w_px_num_) * dy;
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
    return px_rays;
  }
  /** @return background color (default black)*/
  static constexpr const Color& GetEmptyColor() { return EMPTY_COLOR; }
  /** @return picture left coordinate edge (default 0.0)*/
  constexpr double GetLeftCoor() const { return left_coor_; }
  /** @return picture right coordinate edge (default 600.0)*/
  constexpr double GetRightCoor() const { return right_coor_; }
  /** @return picture top coordinate edge (default 0.0)*/
  constexpr double GetTopCoor() const { return top_coor_; }
  /** @return picture bottom coordinate edge (default 400.0)*/
  constexpr double GetBotCoor() const { return bot_coor_; }
  /** @return picture width in pixel (default 600)*/
  constexpr uint32_t GetWidthInPx() const { return w_px_num_; }
  /** @return picture height in pixel (default 400)*/
  constexpr uint32_t GetHeightInPx() const { return h_px_num_; }
  /** @return total number of pixels in the picture (default 600*400)*/
  constexpr uint32_t GetPxNum() const { return w_px_num_ * h_px_num_; }
  /** @return distance from camera to the picture plane (default 500.0)*/
  constexpr double GetDistToPlane() const { return dist_to_plane_; }
};

inline size_t Camera::SAMPLES_PER_PIXEL = 10;
inline Color Camera::EMPTY_COLOR = Color{0, 0, 0};
#endif  // CAMERA_H

#ifndef RAY_H
#define RAY_H

#include <utility>

#include "GeoVec.h"
/** Ray reprasentation as point and direction. Direction in this class is
 * guaranteed to be normalized*/
class Ray {
  GeoVec pos_;
  GeoVec dir_;

 public:
  Ray() = delete;
  Ray(GeoVec pos, GeoVec dir)
      : pos_(std::move(pos)), dir_(std::move(dir.Norm())) {}
  /** Moves ray start point to the given distance along its direction*/
  constexpr Ray& Advance(double dist) {
    assert(dist > 0);
    pos_ = pos_ + dist * dir_;
    return *this;
  }
  /** Moves ray start point on the given distance in the direction opposite to
   * the ray direction*/
  constexpr Ray& StepBack(double dist) {
    assert(dist > 0);
    pos_ = pos_ - dist * dir_;
    return *this;
  }
  /** Sets new direction and normalizes it*/
  Ray& UpdateDirection(const GeoVec& new_dir) {
    dir_ = new_dir;
    dir_.Norm();
    return *this;
  }

  constexpr const GeoVec& GetPos() const { return pos_; }
  constexpr const GeoVec& GetDir() const { return dir_; }
};

#endif  // RAY_H

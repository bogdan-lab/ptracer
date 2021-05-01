#ifndef GEO_VEC_H
#define GEO_VEC_H

#include <assert.h>

#include <cmath>

struct GeoVec {
  double x_, y_, z_;

  GeoVec() = delete;
  GeoVec(double x, double y, double z) : x_(x), y_(y), z_(z) {}
  GeoVec(const GeoVec& start, const GeoVec& end) {
    x_ = end.x_ - start.x_;
    y_ = end.y_ - start.y_;
    z_ = end.z_ - start.z_;
  }

  double Len() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }

  GeoVec& Norm() {
    assert(Len() != 0);
    double mult = 1.0 / Len();
    x_ *= mult;
    y_ *= mult;
    z_ *= mult;
    return *this;
  }

  double Dot(const GeoVec& other) const {
    return other.x_ * x_ + other.y_ * y_ + other.z_ * z_;
  }

  GeoVec Cross(const GeoVec& other) const {
    return {y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_};
  }
};

inline GeoVec operator/(const GeoVec& lhs, double rhs) {
  assert(rhs != 0);
  return {lhs.x_ / rhs, lhs.y_ / rhs, lhs.z_ / rhs};
}

inline GeoVec operator*(const GeoVec& lhs, double rhs) {
  return {lhs.x_ * rhs, lhs.y_ * rhs, lhs.z_ * rhs};
}

inline GeoVec operator*(double lhs, const GeoVec& rhs) { return rhs * lhs; }

inline GeoVec operator+(const GeoVec& lhs, const GeoVec& rhs) {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

inline GeoVec operator-(const GeoVec& lhs, const GeoVec& rhs) {
  return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_};
}

inline bool operator==(const GeoVec& lhs, const GeoVec& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
}

inline bool operator!=(const GeoVec& lhs, const GeoVec& rhs) {
  return lhs.x_ != rhs.x_ || lhs.y_ != rhs.y_ || lhs.z_ == rhs.z_;
}

inline GeoVec operator-(const GeoVec& v) { return {-v.x_, -v.y_, -v.z_}; }

inline double dist_btw_points(const GeoVec& lhs, const GeoVec& rhs) {
  GeoVec tmp{lhs, rhs};
  return tmp.Len();
}

#endif  // GEO_VEC_H

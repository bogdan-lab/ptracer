/**
 * @file GeoVec.h
 * Contain utilities to work with geometrical vector
 */
#ifndef GEO_VEC_H
#define GEO_VEC_H

#include <assert.h>

#include <cmath>
#include <limits>
/**
 * Simple realization of an geometrical vector
 */
struct GeoVec {
  double x_ = std::numeric_limits<double>::quiet_NaN();
  double y_ = std::numeric_limits<double>::quiet_NaN();
  double z_ = std::numeric_limits<double>::quiet_NaN();

  /** By defaul all coordinate values are NaN*/
  constexpr GeoVec() = default;
  /** Creates vector using passed coordinate values*/
  constexpr GeoVec(double x, double y, double z) : x_(x), y_(y), z_(z) {}
  /** Creates vector connecting two points - start and end*/
  constexpr GeoVec(const GeoVec& start, const GeoVec& end) {
    x_ = end.x_ - start.x_;
    y_ = end.y_ - start.y_;
    z_ = end.z_ - start.z_;
  }
  /** @return length of the vector*/
  double Len() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
  /** Normalizes current vector to have length equal to 1*/
  GeoVec& Norm() {
    assert(Len() != 0);
    double mult = 1.0 / Len();
    x_ *= mult;
    y_ *= mult;
    z_ *= mult;
    return *this;
  }
  /** @return dot prodact of current vector and the given one*/
  constexpr double Dot(const GeoVec& other) const {
    return other.x_ * x_ + other.y_ * y_ + other.z_ * z_;
  }
  /** @return cross product between current vector and the given one*/
  constexpr GeoVec Cross(const GeoVec& other) const {
    return {y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_};
  }
};

inline constexpr GeoVec operator/(const GeoVec& lhs, double rhs) {
  assert(rhs != 0);
  return {lhs.x_ / rhs, lhs.y_ / rhs, lhs.z_ / rhs};
}

inline constexpr GeoVec operator*(const GeoVec& lhs, double rhs) {
  return {lhs.x_ * rhs, lhs.y_ * rhs, lhs.z_ * rhs};
}

inline constexpr GeoVec operator*(double lhs, const GeoVec& rhs) {
  return rhs * lhs;
}

inline constexpr GeoVec operator+(const GeoVec& lhs, const GeoVec& rhs) {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

inline constexpr GeoVec operator-(const GeoVec& lhs, const GeoVec& rhs) {
  return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_};
}

inline constexpr bool operator==(const GeoVec& lhs, const GeoVec& rhs) {
  return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
}

inline constexpr bool operator!=(const GeoVec& lhs, const GeoVec& rhs) {
  return lhs.x_ != rhs.x_ || lhs.y_ != rhs.y_ || lhs.z_ == rhs.z_;
}

inline constexpr GeoVec operator-(const GeoVec& v) {
  return {-v.x_, -v.y_, -v.z_};
}
/** @return distance between two given points*/
inline double dist_btw_points(const GeoVec& lhs, const GeoVec& rhs) {
  GeoVec tmp{lhs, rhs};
  return tmp.Len();
}

#endif  // GEO_VEC_H

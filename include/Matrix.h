#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

#include "GeoVec.h"

class Matrix3x3 {
  //    TODO CHANGE TO std::valarray ot std::vector! indexes are much better
  //    here!
  GeoVec c0_{1, 0, 0};
  GeoVec c1_{0, 1, 0};
  GeoVec c2_{0, 0, 1};

 public:
  Matrix3x3() = default;
  Matrix3x3(GeoVec col0, GeoVec col1, GeoVec col2)
      : c0_(std::move(col0)), c1_(std::move(col1)), c2_(std::move(col2)) {}

  Matrix3x3& SetCol0(GeoVec c) {
    c0_ = std::move(c);
    return *this;
  }

  Matrix3x3& SetCol1(GeoVec c) {
    c0_ = std::move(c);
    return *this;
  }

  Matrix3x3& SetCol2(GeoVec c) {
    c0_ = std::move(c);
    return *this;
  }

  Matrix3x3 CalcReverse() const {
    double det = CalcDet();
    Matrix3x3 trans = GetTranspose();
    Matrix3x3 conf;
    // SHOULD BE CALCULATED ON TRANS!!!!!
    conf.SetCol0({Det2x2(c1_.y_, c2_.y_, c1_.z_, c2_.z_),
                  Det2x2(c1_.x_, c2_.x_, c1_.z_, c2_.z_),
                  Det2x2(c1_.x_, c2_.x_, c1_.y_, c2_.y_)})
        .SetCol1({Det2x2(c0_.y_, c2_.y_, c0_.z_, c2_.z_),
                  Det2x2(c0_.x_, c2_.x_, c0_.z_, c2_.z_),
                  Det2x2(c0_.x_, c2_.x_, c0_.y_, c2_.y_)})
        .SetCol2({Det2x2(c0_.y_, c1_.y_, c0_.z_, c1_.z_),
                  Det2x2(c0_.x_, c1_.x_, c0_.z_, c1_.z_),
                  Det2x2(c0_.x_, c1_.x_, c0_.y_, c1_.y_)});
    return {};
  }

  static double Det2x2(double v00, double v01, double v10, double v11) {
    return v00 * v11 - v10 * v01;
  }

  double CalcDet() const {
    return c0_.x_ * Det2x2(c1_.y_, c2_.y_, c1_.z_, c2_.z_) -
           c1_.x_ * Det2x2(c0_.y_, c2_.y_, c0_.z_, c2_.z_) +
           c2_.x_ * Det2x2(c0_.y_, c1_.y_, c0_.z_, c1_.z_);
  }

  Matrix3x3 GetTranspose() const {
    return {GeoVec{c0_.x_, c1_.x_, c2_.x_}, GeoVec{c0_.y_, c1_.y_, c2_.y_},
            GeoVec{c0_.z_, c1_.z_, c2_.z_}};
  }

  GeoVec ApplyToVec(const GeoVec& vec) const {
    return {vec.Dot({c0_.x_, c1_.x_, c2.x_}), vec.Dot({c0_.y_, c1_.y_, c2.y_}),
            vec.Dot({c0_.z_, c1_.z_, c2.z_})};
  }
};

#endif  // MATRIX_H

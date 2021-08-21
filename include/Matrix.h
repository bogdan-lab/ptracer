#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

#include "GeoVec.h"

class Matrix3x3 {
  GeoVec c0_;
  GeoVec c1_;
  GeoVec c2_;

  static double Det2x2(double v00, double v01, double v10, double v11) {
    return v00 * v11 - v10 * v01;
  }

 public:
  Matrix3x3(GeoVec p0, GeoVec p1, GeoVec p2) : c0_(p0), c1_(p1), c2_(p2) {}

  Matrix3x3 CalcReverse() const {
    double det = CalcDet();
    Matrix3x3 tr = GetTranspose();

    double a00 = Det2x2(tr.c1_.y_, tr.c2_.y_, tr.c1_.z_, tr.c2_.z_) / det;
    double a01 = -Det2x2(tr.c0_.y_, tr.c2_.y_, tr.c0_.z_, tr.c2_.z_) / det;
    double a02 = Det2x2(tr.c0_.y_, tr.c1_.y_, tr.c0_.z_, tr.c1_.z_) / det;

    double a10 = -Det2x2(tr.c1_.x_, tr.c2_.x_, tr.c1_.z_, tr.c2_.z_) / det;
    double a11 = Det2x2(tr.c0_.x_, tr.c2_.x_, tr.c0_.z_, tr.c2_.z_) / det;
    double a12 = -Det2x2(tr.c0_.x_, tr.c1_.x_, tr.c0_.z_, tr.c1_.z_) / det;

    double a20 = Det2x2(tr.c1_.x_, tr.c2_.x_, tr.c1_.y_, tr.c2_.y_) / det;
    double a21 = -Det2x2(tr.c0_.x_, tr.c2_.x_, tr.c0_.y_, tr.c2_.y_) / det;
    double a22 = Det2x2(tr.c0_.x_, tr.c1_.x_, tr.c0_.y_, tr.c1_.y_) / det;
    return {GeoVec{a00, a10, a20}, GeoVec{a01, a11, a21},
            GeoVec{a02, a12, a22}};
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
    return {vec.Dot({c0_.x_, c1_.x_, c2_.x_}),
            vec.Dot({c0_.y_, c1_.y_, c2_.y_}),
            vec.Dot({c0_.z_, c1_.z_, c2_.z_})};
  }
};

#endif  // MATRIX_H

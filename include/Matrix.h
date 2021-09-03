#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

#include "GeoVec.h"

struct Matrix3x3 {
  GeoVec c0;
  GeoVec c1;
  GeoVec c2;
};

inline constexpr double Det2x2(double v00, double v01, double v10, double v11) {
  return v00 * v11 - v10 * v01;
}

inline constexpr double Det3x3(const Matrix3x3& m) {
  return m.c0.x_ * Det2x2(m.c1.y_, m.c2.y_, m.c1.z_, m.c2.z_) -
         m.c1.x_ * Det2x2(m.c0.y_, m.c2.y_, m.c0.z_, m.c2.z_) +
         m.c2.x_ * Det2x2(m.c0.y_, m.c1.y_, m.c0.z_, m.c1.z_);
}

inline constexpr Matrix3x3 GetTranspose(const Matrix3x3& m) {
  return {GeoVec{m.c0.x_, m.c1.x_, m.c2.x_}, GeoVec{m.c0.y_, m.c1.y_, m.c2.y_},
          GeoVec{m.c0.z_, m.c1.z_, m.c2.z_}};
}

inline constexpr GeoVec ApplyToVec(const Matrix3x3& m, const GeoVec& vec) {
  return {vec.Dot({m.c0.x_, m.c1.x_, m.c2.x_}),
          vec.Dot({m.c0.y_, m.c1.y_, m.c2.y_}),
          vec.Dot({m.c0.z_, m.c1.z_, m.c2.z_})};
}

inline constexpr Matrix3x3 GetReverse3x3(const Matrix3x3& m) {
  double det = Det3x3(m);
  Matrix3x3 tr = GetTranspose(m);

  double a00 = Det2x2(tr.c1.y_, tr.c2.y_, tr.c1.z_, tr.c2.z_) / det;
  double a01 = -Det2x2(tr.c0.y_, tr.c2.y_, tr.c0.z_, tr.c2.z_) / det;
  double a02 = Det2x2(tr.c0.y_, tr.c1.y_, tr.c0.z_, tr.c1.z_) / det;

  double a10 = -Det2x2(tr.c1.x_, tr.c2.x_, tr.c1.z_, tr.c2.z_) / det;
  double a11 = Det2x2(tr.c0.x_, tr.c2.x_, tr.c0.z_, tr.c2.z_) / det;
  double a12 = -Det2x2(tr.c0.x_, tr.c1.x_, tr.c0.z_, tr.c1.z_) / det;

  double a20 = Det2x2(tr.c1.x_, tr.c2.x_, tr.c1.y_, tr.c2.y_) / det;
  double a21 = -Det2x2(tr.c0.x_, tr.c2.x_, tr.c0.y_, tr.c2.y_) / det;
  double a22 = Det2x2(tr.c0.x_, tr.c1.x_, tr.c0.y_, tr.c1.y_) / det;
  return {GeoVec{a00, a10, a20}, GeoVec{a01, a11, a21}, GeoVec{a02, a12, a22}};
}

#endif  // MATRIX_H

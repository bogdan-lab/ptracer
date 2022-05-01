/**
 * @file Matrix.h
 * Contain utilities to work with 3x3 matrixes
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <utility>

#include "GeoVec.h"
/** 3x3 matrix representation. Each field represent corresponding column*/
struct Matrix3x3 {
  GeoVec c0;
  GeoVec c1;
  GeoVec c2;
};
/** Calculates deterimnant of the 2x2 matrix. input - separate matrix values*/
inline constexpr double Det2x2(double v00, double v01, double v10, double v11) {
  return v00 * v11 - v10 * v01;
}
/** Calculates determinatn of the given 3x3 matrix*/
inline constexpr double Det3x3(const Matrix3x3& m) {
  return m.c0.x_ * Det2x2(m.c1.y_, m.c2.y_, m.c1.z_, m.c2.z_) -
         m.c1.x_ * Det2x2(m.c0.y_, m.c2.y_, m.c0.z_, m.c2.z_) +
         m.c2.x_ * Det2x2(m.c0.y_, m.c1.y_, m.c0.z_, m.c1.z_);
}
/** Returns transpose matrix obtained from the give one*/
inline constexpr Matrix3x3 GetTranspose(const Matrix3x3& m) {
  return {GeoVec{m.c0.x_, m.c1.x_, m.c2.x_}, GeoVec{m.c0.y_, m.c1.y_, m.c2.y_},
          GeoVec{m.c0.z_, m.c1.z_, m.c2.z_}};
}
/**
 * Separately calculates value which will ocure in the first row of the given
 * vector after applying to it given matrix
 */
inline constexpr double TransformXCoor(const Matrix3x3& m, const GeoVec& vec) {
  return vec.Dot({m.c0.x_, m.c1.x_, m.c2.x_});
}
/**
 * Separately calculates value which will ocure in the second row of the given
 * vector after applying to it given matrix
 */
inline constexpr double TransformYCoor(const Matrix3x3& m, const GeoVec& vec) {
  return vec.Dot({m.c0.y_, m.c1.y_, m.c2.y_});
}
/**
 * Separately calculates value which will ocure in the third row of the given
 * vector after applying to it given matrix
 */
inline constexpr double TransformZCoor(const Matrix3x3& m, const GeoVec& vec) {
  return vec.Dot({m.c0.z_, m.c1.z_, m.c2.z_});
}
/** @return result vector after applying given matrix to the given vector*/
inline constexpr GeoVec ApplyToVec(const Matrix3x3& m, const GeoVec& vec) {
  return {TransformXCoor(m, vec), TransformYCoor(m, vec),
          TransformZCoor(m, vec)};
}
/** Calculates reverse 3x3 matrix to the given one*/
Matrix3x3 GetReverse3x3(const Matrix3x3& m);

inline constexpr Matrix3x3 operator*(const Matrix3x3& lhs,
                                     const Matrix3x3& rhs) {
  return {ApplyToVec(lhs, rhs.c0), ApplyToVec(lhs, rhs.c1),
          ApplyToVec(lhs, rhs.c2)};
}

#endif  // MATRIX_H

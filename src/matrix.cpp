#include "Matrix.h"

Matrix3x3 GetReverse3x3(const Matrix3x3& m) {
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

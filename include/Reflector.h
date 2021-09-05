﻿#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <random>

#include "GeoVec.h"

inline GeoVec MirrorReflect(const GeoVec& dir, const GeoVec& norm) {
  assert(dir.Dot(norm) < 0);
  return dir - 2 * dir.Dot(norm) * norm;
}

inline GeoVec DiffuseReflect(std::mt19937& rnd, const GeoVec& dir,
                             const GeoVec& norm) {
  assert(dir.Dot(norm) < 0);
  std::uniform_real_distribution<double> phi_gen{0, 2 * M_PI};
  GeoVec y_ort = norm.Cross(dir);
  if (y_ort.x_ == 0 && y_ort.y_ == 0 && y_ort.z_ == 0) {
    y_ort = norm.Cross(dir + GeoVec{phi_gen(rnd), phi_gen(rnd), phi_gen(rnd)});
  }
  y_ort.Norm();
  GeoVec x_ort = y_ort.Cross(norm).Norm();
  double phi = phi_gen(rnd);
  std::uniform_real_distribution<double> cos_th_gen{0, 1.0};
  double cos_theta = cos_th_gen(rnd);
  return norm * cos_theta + std::sqrt(1 - cos_theta * cos_theta) *
                                (std::sin(phi) * x_ort + std::cos(phi) * y_ort);
}

inline GeoVec HybridReflect(std::mt19937& rnd, double polishness,
                            const GeoVec& dir, const GeoVec& norm) {
  assert(polishness >= 0);
  assert(polishness <= 1);
  std::uniform_real_distribution<double> dist{0, 1};
  if (dist(rnd) < polishness) {
    return MirrorReflect(dir, norm);
  }
  return DiffuseReflect(rnd, dir, norm);
}

#endif  // REFLECTOR_H

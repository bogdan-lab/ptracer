#include <random>

#include "GeoVec.h"

GeoVec MirrorReflect(const GeoVec& dir, const GeoVec& norm) {
  assert(dir.Dot(norm) < 0);
  return dir - 2 * dir.Dot(norm) * norm;
}

GeoVec DiffuseReflect(std::mt19937& rnd, const GeoVec& dir,
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
  double sin_phi = std::sin(phi);
  double cos_phi = std::cos(phi);
  std::uniform_real_distribution<double> cos_th_gen{0, 1.0};
  double cos_theta = cos_th_gen(rnd);
  double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
  return norm * cos_theta + sin_theta * (sin_phi * x_ort + cos_phi * y_ort);
}

GeoVec HybridReflect(std::mt19937& rnd, double polishness, const GeoVec& dir,
                     const GeoVec& norm) {
  assert(polishness >= 0);
  assert(polishness <= 1);
  std::uniform_real_distribution<double> dist{0, 1};
  if (dist(rnd) < polishness) {
    return MirrorReflect(dir, norm);
  }
  return DiffuseReflect(rnd, dir, norm);
}

#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <random>

#include "GeoVec.h"
/**
 * Claculates mirror like reflection of an inciden ray
 * Method expects that ray hits the surface - it does not perform additional
 * checks for it returned direction is not normed
 * @param dir - initial direction of the ray
 * @param norm - reflective usrface normal
 *
 * @return direction obtained by dreflection
 */
inline GeoVec MirrorReflect(const GeoVec& dir, const GeoVec& norm) {
  assert(dir.Dot(norm) < 0);
  return dir - 2 * dir.Dot(norm) * norm;
}
/**
 * Claculates random reflection of an incident ray
 * Method expects that ray hits the surface - it does not perform additional
 * checks for it. Returned direction is not normed
 * @param dir - incident ray direction
 * @param norm - reflective surface normal
 *
 * @return direction obtained by reflection
 */
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
/**
 * Method perform calculation of reflection in mixed manner
 * With some probability (based on polishness) reflection will be calculated for
 * mirror like case or random reflection in 2 pi.
 * Returned direction is not normed
 *
 * @param polishness - defines distribution of the reflection mix. Should be in
 * range [0, 1]. If it is 0 - all rays will be randomly reflected, if it is 1 -
 * all rays will be reflected mirror-like
 * @param dir - initial ray direction
 * @param norm - reflective surface normal
 *
 * @return direction obtained by reflection
 */
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

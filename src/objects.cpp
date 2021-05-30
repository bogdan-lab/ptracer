#include "Objects.h"

#include <cmath>
#include <optional>

std::optional<double> Sphere::GetClosesDist(const Ray& ray) const {
  GeoVec ray_to_c{ray.GetPos(), center_};
  double dist_to_c = ray_to_c.Len();
  assert(dist_to_c > r_);  // should not be inside sphere
  double dist_proj = ray_to_c.Dot(ray.GetDir());
  if (dist_proj <= 0) return std::nullopt;  // ray directed away!
  double D_quarter = dist_proj * dist_proj - dist_to_c * dist_to_c + r_ * r_;
  if (D_quarter <= 0)
    return std::nullopt;  //==0 - ignore when ray only touch the surface
  return dist_proj - std::sqrt(D_quarter);
}

std::optional<double> Triangle::GetClosesDist(const Ray& ray) const {
  // find ray crossection with the plain
  const auto& ray_dir = ray.GetDir();
  const auto& ray_pos = ray.GetPos();
  double den = ray_dir.Dot(norm_);
  if (den >= 0) return std::nullopt;  // ray is directed away!
  double num = ray_pos.Dot(norm_);
  double t = -(D_ + num) / den;
  if (!CheckInTriangle(ray_pos + t * ray_dir)) return std::nullopt;
  return t;
}

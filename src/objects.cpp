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

Matrix3x3 Triangle::CalcNormOperator(const GeoVec& p0, const GeoVec& p1,
                                     const GeoVec& p2) {
  // if three points are on the same plane need to change basis
  Matrix3x3 init_triangle{p0, p1, p2};
  if (Det3x3(init_triangle) == 0) {
    GeoVec origin = (p0 + p1 + p2) / 3.0;
    if (p0.x_ == p1.x_ && p0.x_ == p2.x_) {
      origin = origin + GeoVec(1, 0, 0);
    } else if (p0.y_ == p1.y_ && p0.y_ == p2.y_) {
      origin = origin + GeoVec(0, 1, 0);
    } else {
      origin = origin + GeoVec(0, 0, 1);
    }
    GeoVec x_vec{origin, p0};
    GeoVec y_vec{x_vec.Cross(GeoVec{origin, p1})};
    GeoVec z_vec{x_vec.Cross(y_vec)};

    Matrix3x3 new_basis{x_vec, y_vec, z_vec};
    Matrix3x3 basis_converter = GetReverse3x3(new_basis);
    Matrix3x3 triangle{ApplyToVec(basis_converter, p0),
                       ApplyToVec(basis_converter, p1),
                       ApplyToVec(basis_converter, p2)};
    return GetReverse3x3(triangle) * basis_converter;
  }
  return GetReverse3x3(init_triangle);
}

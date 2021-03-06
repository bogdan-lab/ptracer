#include "Objects.h"

#include <algorithm>
#include <cmath>
#include <optional>

namespace {
Matrix3x3 MkTriangleBasis(const GeoVec& p0, const GeoVec& p1,
                          const GeoVec& p2) {
  GeoVec x{p0, p1};
  GeoVec z = x.Cross({p0, p2});
  GeoVec y = z.Cross(x);
  return {x, y, z};
}

Line CalcLineParameters(double x1, double y1, double x2, double y2) {
  double k = (y2 - y1) / (x2 - x1);
  double b = y2 - k * x2;
  return {k, b};
}
}  // namespace

InTriangleChecker Triangle::CalcInTriangleChecker(const GeoVec& p0,
                                                  const GeoVec& p1,
                                                  const GeoVec& p2) {
  InTriangleChecker checker;
  checker.conversion_matrix = GetReverse3x3(MkTriangleBasis(p0, p1, p2));

  checker.x_sorted[0] = checker.y_sorted[0] =
      ApplyToVec(checker.conversion_matrix, p0);
  checker.x_sorted[1] = checker.y_sorted[1] =
      ApplyToVec(checker.conversion_matrix, p1);
  checker.x_sorted[2] = checker.y_sorted[2] =
      ApplyToVec(checker.conversion_matrix, p2);

  std::sort(checker.x_sorted.begin(), checker.x_sorted.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.x_ < rhs.x_; });

  std::sort(checker.y_sorted.begin(), checker.y_sorted.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.y_ < rhs.y_; });

  Matrix3x3 shear{{1, 0, 0},
                  {-(checker.y_sorted[2].x_ - checker.y_sorted[0].x_) /
                       (checker.y_sorted[2].y_ - checker.y_sorted[0].y_),
                   1, 0},
                  {0, 0, 1}};

  for (auto& el : checker.x_sorted) {
    el = ApplyToVec(shear, el);
  }
  std::sort(checker.x_sorted.begin(), checker.x_sorted.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.x_ < rhs.x_; });
  for (auto& el : checker.y_sorted) {
    el = ApplyToVec(shear, el);
  }

  checker.conversion_matrix = shear * checker.conversion_matrix;

  checker.y_min_mid =
      CalcLineParameters(checker.y_sorted[0].x_, checker.y_sorted[0].y_,
                         checker.y_sorted[1].x_, checker.y_sorted[1].y_);
  checker.y_mid_max =
      CalcLineParameters(checker.y_sorted[1].x_, checker.y_sorted[1].y_,
                         checker.y_sorted[2].x_, checker.y_sorted[2].y_);
  return checker;
}

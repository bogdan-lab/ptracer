#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <memory>
#include <optional>
#include <random>

#include "Color.h"
#include "GeoVec.h"
#include "Matrix.h"
#include "Ray.h"
#include "Reflector.h"

// TODO here is an opportunity to add kRefractive material
enum class Material { kNoMaterial = 0, kReflective, kLightSource };

class Object {
 private:
  double polishness_ = 0.0;
  double refl_coef_ = 1.0;
  double hit_precision_ = 1e-9;
  Color color_ = colors::kNoColor;
  Material mat_ = Material::kNoMaterial;
  mutable std::mt19937 rnd_{0};

 public:
  constexpr Object& SetColor(Color col) {
    color_ = std::move(col);
    return *this;
  }

  constexpr Object& SetMaterial(Material g_mat) {
    mat_ = g_mat;
    return *this;
  }

  constexpr Object& SetHitPrecision(double prc) {
    hit_precision_ = prc;
    return *this;
  }

  Object& SetPolishness(double g_pol) {
    if (g_pol < 0 || g_pol > 1)
      throw std::logic_error("Incorrect polishnaess value. Should be in [0,1]");
    polishness_ = g_pol;
    return *this;
  }

  Object& SetReflectionCoef(double g_r) {
    if (g_r < 0 || g_r > 1)
      throw std::logic_error(
          "Incorrect reflection coefficient. Should be in [0,1]");
    refl_coef_ = g_r;
    return *this;
  }

  Object& SetSeed(size_t s) {
    rnd_.seed(s);
    return *this;
  }

  constexpr const Color& GetColor() const { return color_; }
  constexpr Material GetMaterial() const { return mat_; }
  constexpr double GetHitPrecision() const { return hit_precision_; }
  constexpr double GetPolishness() const { return polishness_; }
  constexpr double GetReflectionCoefficient() const { return refl_coef_; }

  GeoVec DoReflection(const GeoVec& dir, const GeoVec& norm) const {
    return HybridReflect(rnd_, polishness_, dir, norm);
  }

  bool TryReflect(Ray& ray, double dist) {
    assert(refl_coef_ >= 0);
    assert(refl_coef_ <= 1);
    std::uniform_real_distribution<double> d{0, 1};
    if (d(rnd_) < refl_coef_) {
      Reflect(ray, dist);
      return true;
    }
    return false;
  }

  virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
  virtual GeoVec GetNorm(const GeoVec& p) const = 0;
  virtual void Reflect(Ray& ray, double dist) const = 0;
  virtual ~Object() = default;
};

class Sphere : public Object {
 private:
  GeoVec center_;
  double r_;

 public:
  Sphere() = delete;
  constexpr Sphere(const GeoVec& gc, double gr) : center_(gc), r_(gr) {
    assert(r_ > 0);
  }

  std::optional<double> GetClosesDist(const Ray& ray) const override;

  GeoVec GetNorm(const GeoVec& p) const override { return (p - center_) / r_; }

  void Reflect(Ray& ray, double dist) const override {
    const auto& ray_dir = ray.GetDir();
    const auto& ray_pos = ray.GetPos();
    assert(dist_btw_points(ray_pos, center_) > r_);
    ray.Advance(dist);
    while (dist_btw_points(ray_pos, center_) <= r_) {
      ray.StepBack(GetHitPrecision());
    }
    assert(dist_btw_points(ray_pos, center_) > r_);
    ray.UpdateDirection(DoReflection(ray_dir, GetNorm(ray_pos)));
  }
};

struct Line {
  double k;
  double b;
};

struct InTriangleChecker {
  Matrix3x3 conversion_matrix;
  // arrays below contain modified points (in triangle basis and sheared)
  std::array<GeoVec, 3> y_sorted;
  std::array<GeoVec, 3> x_sorted;
  Line y_min_mid;
  Line y_mid_max;
};

class Triangle : public Object {
 private:
  GeoVec p0_;
  GeoVec p1_;
  GeoVec p2_;
  GeoVec norm_;
  double D_;  // coefficient for plain Ax + By + Cz + D = 0
  InTriangleChecker checker_;

  InTriangleChecker CalcInTriangleChecker(const GeoVec& p0, const GeoVec& p1,
                                          const GeoVec& p2);

 public:
  Triangle() = delete;
  Triangle(const GeoVec& gp0, const GeoVec& gp1, const GeoVec& gp2)
      : p0_(gp0), p1_(gp1), p2_(gp2) {
    GeoVec lhs{p0_, p1_};
    GeoVec rhs{p0_, p2_};
    norm_ = lhs.Cross(rhs).Norm();
    D_ = -norm_.Dot(p0_);
    checker_ = CalcInTriangleChecker(p0_, p1_, p2_);
  }

  std::optional<double> GetClosesDist(const Ray& ray) const override;

  GeoVec GetNorm(const GeoVec& /*p*/) const override { return norm_; }

  void Reflect(Ray& ray, double dist) const override {
    const auto& ray_dir = ray.GetDir();
    const auto& ray_pos = ray.GetPos();
    assert(ray_dir.Dot(norm_) < 0);
    ray.Advance(dist);
    GeoVec check_vec{p0_, ray_pos};
    while (check_vec.Dot(norm_) < 0) {
      ray.StepBack(GetHitPrecision());
      check_vec = {p0_, ray_pos};
    }
    ray.UpdateDirection(DoReflection(ray_dir, GetNorm(ray_pos)));
  }

  constexpr bool CheckInTriangle(const GeoVec& point) const {
    // TODO assert that point is on the triangle surface!
    GeoVec norm_point = ApplyToVec(checker_.conversion_matrix, point);
    if (norm_point.x_ < checker_.x_sorted[0].x_ ||
        norm_point.x_ > checker_.x_sorted[2].x_ ||
        norm_point.y_ < checker_.y_sorted[0].y_ ||
        norm_point.y_ > checker_.y_sorted[2].y_) {
      return false;
    }
    if (norm_point.y_ < checker_.y_sorted[1].y_) {
      return checker_.y_min_mid.k * norm_point.x_ + checker_.y_min_mid.b <
             norm_point.y_;
    } else {
      return checker_.y_mid_max.k * norm_point.x_ + checker_.y_mid_max.b >
             norm_point.y_;
    }
  }
};

#endif  // OBJECTS_H

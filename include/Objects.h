#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <memory>
#include <optional>
#include <random>

#include "Color.h"
#include "GeoVec.h"
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

class Triangle : public Object {
 private:
  GeoVec p0_;
  GeoVec p1_;
  GeoVec p2_;
  GeoVec norm_;
  double D_;  // coefficient for plain Ax + By + Cz + D = 0

 public:
  Triangle() = delete;
  Triangle(const GeoVec& gp0, const GeoVec& gp1, const GeoVec& gp2)
      : p0_(gp0), p1_(gp1), p2_(gp2) {
    GeoVec lhs{p0_, p1_};
    GeoVec rhs{p0_, p2_};
    norm_ = lhs.Cross(rhs).Norm();
    D_ = -norm_.Dot(p0_);
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
    // TODO looks like this can be improved if we use normalized triangle!
    // TODO assert that point is on the triangle surface!
    GeoVec to_p0{point, p0_};
    GeoVec to_p1{point, p1_};
    GeoVec to_p2{point, p2_};
    GeoVec v0 = to_p0.Cross(to_p1);
    GeoVec v1 = to_p1.Cross(to_p2);
    GeoVec v2 = to_p2.Cross(to_p0);
    if (v0.Dot(norm_) <= 0 || v1.Dot(norm_) <= 0 || v2.Dot(norm_) <= 0)
      return false;
    return true;
  }
};

#endif  // OBJECTS_H

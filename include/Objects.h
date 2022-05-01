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
/** Interface object class*/
class Object {
 private:
  double polishness_ = 0.0;
  double refl_coef_ = 1.0;
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

  /**
   * Sets degree of the polishness. Its value should be in the range [0, 1]
   * Where 0 - not polished - all reflections are random
   * 1 - mirror like polished
   */
  Object& SetPolishness(double g_pol) {
    if (g_pol < 0 || g_pol > 1)
      throw std::logic_error("Incorrect polishnaess value. Should be in [0,1]");
    polishness_ = g_pol;
    return *this;
  }
  /** Reflection coefficient should be in range [0, 1]*/
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
  constexpr double GetPolishness() const { return polishness_; }
  constexpr double GetReflectionCoefficient() const { return refl_coef_; }

  /**
   * Method decides whether ray will be reflected or not based on the object
   * reflection coefficient. If reflection happens, method will move and turn
   * the ray
   * @param ray - in/out incident ray. If reflection happened will contain new
   * direction and new ray position.
   * @param dist - distance from the current point to the reflective surface
   *
   * @return true - if reflection happened, false - otherwise
   */
  bool TryReflect(Ray& ray, double dist) const {
    assert(refl_coef_ >= 0);
    assert(refl_coef_ <= 1);
    std::uniform_real_distribution<double> d{0, 1};
    if (d(rnd_) < refl_coef_) {
      ray.Advance(dist);
      assert(ray.GetDir().Dot(GetNorm(ray.GetPos())) < 0);
      ray.UpdateDirection(HybridReflect(rnd_, polishness_, ray.GetDir(),
                                        GetNorm(ray.GetPos())));
      return true;
    }
    return false;
  }
  /**
   * Returnes minimal distance from the given ray to this object. If ray does
   * not cross this object nullopt is returned
   */
  virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
  /**
   * Returns this object normal in the given point. All normals point to the
   * direction of surface reflection
   */
  virtual GeoVec GetNorm(const GeoVec& p) const = 0;

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

  double GetRadius() const { return r_; }
  const GeoVec& GetCenter() const { return center_; }

  std::optional<double> GetClosesDist(const Ray& ray) const override {
    GeoVec ray_to_c{ray.GetPos(), center_};
    double dist_proj = ray_to_c.Dot(ray.GetDir());
    if (dist_proj <= 0) return std::nullopt;  // ray directed away!
    double dist_to_c = ray_to_c.Len();
    double D_quarter = dist_proj * dist_proj - dist_to_c * dist_to_c + r_ * r_;
    if (D_quarter <= 0)
      return std::nullopt;  //==0 - ignore when ray only touch the surface
    return dist_proj - std::sqrt(D_quarter);
  }

  GeoVec GetNorm(const GeoVec& p) const override { return (p - center_) / r_; }
};
/** Struct for representing line like y = k*x + b*/
struct Line {
  double k;
  double b;
};
/**
 * Utlitiy struct for more effective check whether point is in triangle or not.
 */
struct InTriangleChecker {
  /**
   * Matrix for changing basis from original to triangle. All other fields
   * coordinates are in triangle basis.
   * Triangle basis is the one where z coordinate is 0 for all 3 points in
   * current triangle
   */
  Matrix3x3 conversion_matrix;
  /** Current triangle coordinates in triangle basis sorted acording to y
   * coordinate*/
  std::array<GeoVec, 3> y_sorted;
  /** Current triangle coordinates in triangle basis sorted acording to x
   * coordinate*/
  std::array<GeoVec, 3> x_sorted;
  /** Equation for line connecting y_min and y_mid points from y_sorted*/
  Line y_min_mid;
  /** Equation for line connecting y_mid and y_max points from y_sorted*/
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
  /** Method which prepares checker struct for effective check ehether point are
   * inside the triangle or not*/
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

  std::optional<double> GetClosesDist(const Ray& ray) const override {
    // find ray crossection with the plain
    const auto& ray_dir = ray.GetDir();
    const auto& ray_pos = ray.GetPos();
    double den = ray_dir.Dot(norm_);
    if (den >= 0) return std::nullopt;  // ray is directed away!
    double t = -(D_ + ray_pos.Dot(norm_)) / den;
    if (!CheckInTriangle(ray_pos + t * ray_dir)) return std::nullopt;
    return t;
  }

  const GeoVec& GetPoint0() const { return p0_; }
  const GeoVec& GetPoint1() const { return p1_; }
  const GeoVec& GetPoint2() const { return p2_; }
  GeoVec GetNorm(const GeoVec& /*p*/) const override { return norm_; }

  /** @return true if point is in triangle, false - otherwise*/
  constexpr bool CheckInTriangle(const GeoVec& point) const {
    double norm_x = TransformXCoor(checker_.conversion_matrix, point);
    double norm_y = TransformYCoor(checker_.conversion_matrix, point);
    if (norm_x < checker_.x_sorted[0].x_ || norm_x > checker_.x_sorted[2].x_ ||
        norm_y < checker_.y_sorted[0].y_ || norm_y > checker_.y_sorted[2].y_) {
      return false;
    }
    if (norm_y < checker_.y_sorted[1].y_) {
      return checker_.y_min_mid.k * norm_x + checker_.y_min_mid.b < norm_y;
    } else {
      return checker_.y_mid_max.k * norm_x + checker_.y_mid_max.b > norm_y;
    }
  }
};

#endif  // OBJECTS_H

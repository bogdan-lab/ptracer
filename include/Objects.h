#ifndef OBJECTS_H
#define OBJECTS_H

#include "GeoVec.h"
#include "Ray.h"
#include "Color.h"

#include <optional>
#include <random>
#include <cmath>
#include <memory>

enum class Material {
    kCommon,
    kLightSource,
    kMirror
};


struct Reflector {
    virtual GeoVec operator()(std::mt19937& rnd, const GeoVec& dir, const GeoVec& norm) const = 0;
    virtual ~Reflector() = default;
};

using ReflectorHolder = std::unique_ptr<Reflector>;

struct MirrorReflector : public Reflector {

    GeoVec operator()([[maybe_unused]] std::mt19937& rnd, const GeoVec& dir, const GeoVec& norm) const override {
        assert(dir.Dot(norm)<0);
        return dir - 2*dir.Dot(norm)*norm;
    }
};

struct DiffuseReflector : public Reflector {
    GeoVec operator()(std::mt19937& rnd, const GeoVec& dir, const GeoVec& norm) const override {
        assert(dir.Dot(norm)<0);
        std::uniform_real_distribution<double>phi_gen{0, 2*M_PI};
        GeoVec y_ort = norm.Cross(dir).Norm();
        GeoVec x_ort = y_ort.Cross(norm).Norm();
        double phi = phi_gen(rnd);
        double sin_phi = std::sin(phi);
        double cos_phi = std::cos(phi);
        std::uniform_real_distribution<double> cos_th_gen{0, 1.0};
        double cos_theta = cos_th_gen(rnd);
        double sin_theta = std::sqrt(1 - cos_theta*cos_theta);
        return {
            norm.x_*cos_theta + sin_theta*(sin_phi*x_ort.x_ + cos_phi*y_ort.x_),
            norm.y_*cos_theta + sin_theta*(sin_phi*x_ort.y_ + cos_phi*y_ort.y_),
            norm.z_*cos_theta + sin_theta*(sin_phi*x_ort.z_ + cos_phi*y_ort.z_)
        };
    }
};


class Object {
private:
    ReflectorHolder reflector_;
    double hit_precision_ = 1e-9;
    Color color_;
    Material mat_;
    std::mt19937 rnd_;
public:

    Object& SetReflector(ReflectorHolder new_ref) {
        reflector_.reset(new_ref.release());
        return *this;
    }

    Object& SetColor(const Color& col) {
        color_ = col;
        return *this;
    }

    Object& SetMaterial(Material g_mat) {
        mat_ = g_mat;
        return *this;
    }

    Object& SetHitPrecision(double prc) {
        hit_precision_ = prc;
        return *this;
    }

    Object& SetSeed(size_t s) {
        rnd_.seed(s);
        return *this;
    }

    const Color& GetColor() const {return color_;}
    Material GetMaterial() const {return mat_;}
    double GetHitPrecision() const {return hit_precision_;}
    const Reflector& GetReflector() const {
        assert(reflector_);
        return *reflector_;
    }
    std::mt19937& AccessRnd() {
        return rnd_;
    }


    virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
    virtual GeoVec GetNorm(const GeoVec& p) const = 0;
    virtual void Reflect(Ray& ray, double dist) = 0;
    virtual ~Object() = default;
};

class Sphere : public Object {
private:
    GeoVec center_;
    double r_;
public:
    Sphere() = delete;
    Sphere(const GeoVec& gc, double gr)
        : center_(gc), r_(gr) {
        assert(r_>0);
    }

    std::optional<double> GetClosesDist(const Ray &ray) const override;

    GeoVec GetNorm(const GeoVec& p) const override {
        return (p - center_)/r_;
    }

    void Reflect(Ray &ray, double dist) override {
        const auto& ray_dir = ray.GetDir();
        const auto& ray_pos = ray.GetPos();
        assert(dist_btw_points(ray_pos, center_)>r_);
        ray.Advance(dist);
        while(dist_btw_points(ray_pos, center_)<=r_) {
            ray.StepBack(GetHitPrecision());
        }
        assert(dist_btw_points(ray_pos, center_)>r_);
        GeoVec norm = GetNorm(ray_pos);
        const auto& reflector = GetReflector();
        ray.UpdateDirection(reflector(AccessRnd(), ray_dir, norm));
    }
};

class Triangle : public Object {
private:
    GeoVec p0_;
    GeoVec p1_;
    GeoVec p2_;
    GeoVec norm_;
    double D_; 	//coefficient for plain Ax + By + Cz + D = 0

public:
    Triangle() = delete;
    Triangle(const GeoVec& gp0, const GeoVec& gp1, const GeoVec& gp2)
        : p0_(gp0), p1_(gp1), p2_(gp2), norm_(0,0,0) {
        GeoVec lhs{p0_, p1_};
        GeoVec rhs{p0_, p2_};
        norm_ = lhs.Cross(rhs).Norm();
        D_ = -norm_.Dot(p0_);
    }

    std::optional<double> GetClosesDist(const Ray &ray) const override;

    GeoVec GetNorm(const GeoVec& /*p*/) const override {return norm_;}

    //TODO code duplication looks like Reflect should be defined in separate class and belong to Object field
    void Reflect(Ray& ray, double dist) override {
        const auto& ray_dir = ray.GetDir();
        const auto& ray_pos = ray.GetPos();
        assert(ray_dir.Dot(norm_)<0);
        ray.Advance(dist);
        GeoVec check_vec{p0_, ray_pos};
        while(check_vec.Dot(norm_)<=0) {
            ray.StepBack(GetHitPrecision());
            check_vec = {p0_, ray_pos};
        }
        GeoVec norm = GetNorm(ray_pos);
        const auto& reflector = GetReflector();
        ray.UpdateDirection(reflector(AccessRnd(), ray_dir, norm));

    }

    bool CheckInTriangle(const GeoVec& point) const {
        //TODO looks like this can be improved if we use normalized triangle!
        //TODO assert that point is on the triangle surface!
        GeoVec to_p0{point, p0_};
        GeoVec to_p1{point, p1_};
        GeoVec to_p2{point, p2_};
        GeoVec v0 = to_p0.Cross(to_p1);
        GeoVec v1 = to_p1.Cross(to_p2);
        GeoVec v2 = to_p2.Cross(to_p0);
        if(v0.Dot(norm_)<=0 || v1.Dot(norm_)<=0 || v2.Dot(norm_)<=0)
            return false;
        return true;
    }
};






#endif //OBJECTS_H

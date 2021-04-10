#ifndef OBJECTS_H
#define OBJECTS_H

#include "GeoVec.h"
#include "Ray.h"
#include "Color.h"

#include <optional>


enum class Material {
    kCommon,
    kLightSource,
    kMirror
};



class Object {
private:
    Color color_;
    Material mat_;
    double hit_precision_ = 1e-9;
public:

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

    const Color& GetColor() const {return color_;}
    Material GetMaterial() const {return mat_;}
    double GetHitPrecision() const {return hit_precision_;}

    virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
    virtual GeoVec GetNorm(const GeoVec& p) const = 0;
    virtual void Reflect(Ray& ray_in, double dist) const = 0;
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

    void Reflect(Ray &ray, double dist) const override {
        const auto& ray_dir = ray.GetDir();
        const auto& ray_pos = ray.GetPos();
        assert(dist_btw_points(ray_pos, center_)>r_);
        ray.Advance(dist);
        while(dist_btw_points(ray_pos, center_)<=r_) {
            ray.StepBack(GetHitPrecision());
        }
        assert(dist_btw_points(ray_pos, center_)>r_);
        GeoVec norm = GetNorm(ray_pos);
        ray.UpdateDirection(ray_dir - 2*ray_dir.Dot(norm)*norm);
    }
};








#endif //OBJECTS_H

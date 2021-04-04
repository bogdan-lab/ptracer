﻿#ifndef OBJECTS_H
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
public:

    Object& SetColor(const Color& col) {
        color_ = col;
        return *this;
    }

    Object& SetMaterial(Material g_mat) {
        mat_ = g_mat;
        return *this;
    }

    const Color& GetColor() const {return color_;}
    Material GetMaterial() const {return mat_;}

    virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
    virtual GeoVec GetNorm(const GeoVec& p) const = 0;
    virtual Ray Reflect(const Ray& ray_in, double dist) const = 0;
    virtual ~Object() = default;
};

class Sphere : public Object {
private:
    GeoVec center_;
    double r_;
public:
    Sphere() = delete;
    Sphere(const GeoVec& gc, double gr) : center_(gc), r_(gr) {}

    std::optional<double> GetClosesDist(const Ray &ray) const override;

    GeoVec GetNorm(const GeoVec& p) const override {
        return (p - center_)/r_;
    }

    Ray Reflect(const Ray &ray_in, double dist) const override {
        std::cout << "REFLECTION NOT YET IMPLEMENTED!!!!\n";
        exit(1);
    }
};








#endif //OBJECTS_H
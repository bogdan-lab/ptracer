#ifndef OBJECTS_H
#define OBJECTS_H

#include "GeoVec.h"
#include "Ray.h"

#include <optional>
#include <algorithm>
#include <iostream>

struct Color {
    static const uint8_t MAX_COLOR_VALUE = 255u;
    uint8_t red_{0};
    uint8_t green_{0};
    uint8_t blue_{0};

    Color() = default;

    Color(uint32_t R, uint32_t G, uint32_t B) {
        red_ = std::clamp(R, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
        green_ = std::clamp(G, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
        blue_ = std::clamp(B, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
    }

};

inline std::ostream& operator<<(std::ostream& out, uint8_t val) {
    return out << static_cast<uint32_t>(val);
}

inline std::ostream& operator << (std::ostream& out, const Color& c) {
    return out << c.red_ << ' ' << c.green_ << ' ' << c.blue_;
}


class Object {
private:
    Color color_;
    double emission_{0.0};
public:

    Object& SetColor(const Color& col) {
        color_ = col;
        return *this;
    }

    Object& SetEmission(double g_emit) {
        emission_ = g_emit;
        return *this;
    }

    const Color& GetColor() const {return color_;}
    double GetEmission() const {return emission_;}

    virtual std::optional<double> GetClosesDist(const Ray& ray) const = 0;
    virtual GeoVec GetNorm(const GeoVec& p) const = 0;
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

};


#endif //OBJECTS_H

#ifndef OBJECTS_H
#define OBJECTS_H

#include "GeoVec.h"
#include "Ray.h"

struct Color {
    double red_;
    double green_;
    double blue_;
};


struct Object {
    Color color_;
    double emission_;
    //int type

    virtual double GetDistanceToObj(const Ray& ray) const = 0;
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

    double GetDistanceToObj(const Ray &ray) const override;

    GeoVec GetNorm(const GeoVec& p) const override {
        return (p - center_)/r_;
    }

};


#endif //OBJECTS_H

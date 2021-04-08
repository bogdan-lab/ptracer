#ifndef RAY_H
#define RAY_H

#include "GeoVec.h"
#include <utility>

struct Ray{
    GeoVec pos_;
    GeoVec dir_;

    Ray() = delete;
    Ray(const GeoVec& pos, const GeoVec& dir) :
        pos_(pos), dir_(dir) {dir_.Norm();}

    Ray(GeoVec&& pos, GeoVec&& dir) : pos_(std::move(pos)),
        dir_(std::move(dir.Norm())) {} 	 	//TODO check if here we will still get move!

    Ray& Advance(double dist) {
        pos_ = pos_ + dist*dir_;
        return *this;
    }
};


#endif //RAY_H

#ifndef RAY_H
#define RAY_H

#include "GeoVec.h"
#include <utility>

class Ray{
    GeoVec pos_;
    GeoVec dir_;

public:

    Ray() = delete;
    Ray(const GeoVec& pos, const GeoVec& dir) :
        pos_(pos), dir_(dir) {dir_.Norm();}

    Ray(GeoVec&& pos, GeoVec&& dir) : pos_(std::move(pos)),
        dir_(std::move(dir.Norm())) {}

    Ray& Advance(double dist) {
        assert(dist>0);
        pos_ = pos_ + dist*dir_;
        return *this;
    }

    Ray& StepBack(double dist) {
        assert(dist>0);
        pos_ = pos_ - dist*dir_;
        return *this;
    }

    Ray& UpdateDirection(const GeoVec& new_dir){
        dir_ = new_dir;
        dir_.Norm();
        return *this;
    }

    const GeoVec& GetPos() const { return pos_; }
    const GeoVec& GetDir() const { return dir_; }
};


#endif //RAY_H

#ifndef CAMERA_H
#define CAMERA_H

#include "GeoVec.h"

#include <cstdlib>
#include <random>

class Camera {
private:
    size_t width_{600};
    size_t height_{400};
    double dist_to_plane_{1.0};
    std::mt19937 rnd_;
    std::uniform_real_distribution<double> distrib_{0.0, 1.0};
    GeoVec pos_;

public:
    Camera() : rnd_(42), pos_(width_/2, height_/2, -dist_to_plane_) {}

    Camera(size_t seed) : rnd_(seed), pos_(width_/2, height_/2, -dist_to_plane_)  {}

    GeoVec GetCameraRay() {
        double x_rnd = distrib_(rnd_);
        double y_rnd = distrib_(rnd_);
        return {x_rnd*width_ - pos_.x_,
                y_rnd*height_ - pos_.y_,
                dist_to_plane_};
    }


};


#endif  //CAMERA_H

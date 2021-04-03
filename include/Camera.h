#ifndef CAMERA_H
#define CAMERA_H

#include "GeoVec.h"
#include "Ray.h"
#include "Pixel.h"

#include <cstdlib>
#include <random>
#include <vector>

class Camera {
private:
    size_t width_{600};
    size_t height_{400};
    double dist_to_plane_{500};
    std::mt19937 rnd_;
    std::uniform_real_distribution<double> distrib_{0.0, 1.0};
    GeoVec pos_;

    Ray CreateRay(size_t x_coor, size_t y_coor) const {
        GeoVec dir = {x_coor - pos_.x_, y_coor - pos_.y_, dist_to_plane_};
        return {pos_, dir};
    }

public:
    Camera() : rnd_(42), pos_(width_/2, height_/2, -dist_to_plane_) {}

    explicit Camera(size_t seed) : rnd_(seed), pos_(width_/2, height_/2, -dist_to_plane_)  {}

    Ray GetCameraRay() {
        double x_rnd = distrib_(rnd_);
        double y_rnd = distrib_(rnd_);
        return CreateRay(x_rnd*width_, y_rnd*height_);
    }

    std::vector<Ray> GetCameraRayGrid() const {
        std::vector<Ray> all_rays;
        all_rays.reserve(width_*height_);
        for(size_t row=0; row<height_; row++){
            for(size_t col=0; col<width_; col++){
                all_rays.push_back(CreateRay(col, row));
            }
        }
        return all_rays;
    }

    size_t GetWidth() const {return width_;}
    size_t GetHeight() const {return height_;}
    double GetDistToPlane() const {return dist_to_plane_;}

};


#endif  //CAMERA_H

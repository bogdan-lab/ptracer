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
    //TODO change width_ and height_ to double ->
    //less static_casts + smoothing for pixel by some characteristic radius
    size_t width_{600};
    size_t height_{400};
    double dist_to_plane_{500};
    GeoVec pos_;
public:

    Camera() :
        pos_(static_cast<double>(width_/2),
             static_cast<double>(height_/2),
             -dist_to_plane_)
    {}

    Camera(size_t w, size_t h, double d) : width_(w), height_(h),
        dist_to_plane_(d),
        pos_(static_cast<double>(w/2),
             static_cast<double>(h/2),
             -d)
    {}

    Ray MakeCameraRay(size_t x_coor, size_t y_coor) const {
        GeoVec dir = {static_cast<double>(x_coor) - pos_.x_,
                      static_cast<double>(y_coor) - pos_.y_,
                      dist_to_plane_};
        return {pos_, dir};
    }


    std::vector<Pixel> MakeAllPixels() const {
        std::vector<Pixel> all_pixels;
        size_t px_num = width_*height_;
        all_pixels.reserve(px_num);
        for(size_t i=0; i<px_num; i++) { //TODO do I loose vectorization because of % here?
            all_pixels.emplace_back(MakeCameraRay(i%width_, i/width_));
        }
        return all_pixels;
    }


    size_t GetWidth() const {return width_;}
    size_t GetHeight() const {return height_;}
    double GetDistToPlane() const {return dist_to_plane_;}

};


#endif  //CAMERA_H

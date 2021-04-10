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
    static size_t SAMPLES_PER_PIXEL;
    size_t width_{600};
    size_t height_{400};
    double dist_to_plane_{500};
    //fields for smoothing pixels. Each pixel is not a point, but has radius
    mutable std::mt19937 rnd_{0};
    mutable std::uniform_real_distribution<double> r_dist_{0,0};
    mutable std::uniform_real_distribution<double> angle_dist_{0, 2*M_PI};
    GeoVec pos_; //Camera position
public:

    static void SetSampleNum(size_t g_num) {SAMPLES_PER_PIXEL = g_num;}

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

    Camera(size_t w, size_t h, double d, size_t seed, double Rsmooth)
        : width_(w), height_(h), dist_to_plane_(d), rnd_(seed), r_dist_(0, Rsmooth),
        pos_(static_cast<double>(w/2),
             static_cast<double>(h/2),
             -d)
    {}


    Ray MakeCameraRay(size_t x_coor, size_t y_coor) const {
        double r = r_dist_(rnd_);
        double alpha = angle_dist_(rnd_);
        double x = static_cast<double>(x_coor) + r*std::cos(alpha);
        double y = static_cast<double>(y_coor) + r*std::sin(alpha);
        GeoVec dir = {x - pos_.x_,
                      y - pos_.y_,
                      dist_to_plane_};
        return {pos_, dir};
    }

    Pixel CreatePixel(size_t x_coor, size_t y_coor) const {
        std::vector<Ray> px_rays;
        px_rays.reserve(SAMPLES_PER_PIXEL);
        for(size_t i=0; i<SAMPLES_PER_PIXEL; i++) {
            px_rays.emplace_back(MakeCameraRay(x_coor, y_coor));
        }
        return Pixel{std::move(px_rays)};
    }

    std::vector<Pixel> MakeAllPixels() const {
        std::vector<Pixel> all_pixels;
        size_t px_num = width_*height_;
        all_pixels.reserve(px_num);
        for(size_t i=0; i<px_num; i++) { //TODO do I loose vectorization because of % here?
            all_pixels.emplace_back(CreatePixel(i%width_, i/width_));
        }
        return all_pixels;
    }


    size_t GetWidth() const {return width_;}
    size_t GetHeight() const {return height_;}
    double GetDistToPlane() const {return dist_to_plane_;}

};

inline size_t Camera::SAMPLES_PER_PIXEL = 10;

#endif  //CAMERA_H

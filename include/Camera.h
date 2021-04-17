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
    static size_t SAMPLES_PER_PIXEL;
    double width_{600};
    double height_{400};
    double dist_to_plane_{500};
    //fields for smoothing pixels. Each pixel is not a point, but has radius
    mutable std::mt19937 rnd_{0};
    mutable std::uniform_real_distribution<double> r_dist_{0,0};
    mutable std::uniform_real_distribution<double> angle_dist_{0, 2*M_PI};
    GeoVec pos_; //Camera position
public:

    static void SetSamplePerPixel(size_t g_num) {SAMPLES_PER_PIXEL = g_num;}

    Camera() :
        pos_(width_/2, height_/2, -dist_to_plane_)
    {}

    Camera(double w, double h, double d) : width_(w), height_(h),
        dist_to_plane_(d),
        pos_(w/2, h/2, -d)
    {
        assert(width_>0);
        assert(height_>0);
        assert(dist_to_plane_>0);
    }

    Camera& SetRSmooth(double new_r) {
        assert(new_r>0);
        r_dist_ = std::uniform_real_distribution<double>{0, new_r};
        return *this;
    }

    Camera& SetSeed(size_t new_s) {
        rnd_.seed(new_s);
        return *this;
    }


    Ray MakeCameraRay(double x_coor, double y_coor) const {
        double r = r_dist_(rnd_);
        double alpha = angle_dist_(rnd_);
        double x = x_coor + r*std::cos(alpha);
        double y = y_coor + r*std::sin(alpha);
        GeoVec dir = {x - pos_.x_,
                      y - pos_.y_,
                      dist_to_plane_};
        return {pos_, dir};
    }

    Pixel CreatePixel(double x_coor, double y_coor) const {
        std::vector<Ray> px_rays;
        px_rays.reserve(SAMPLES_PER_PIXEL);
        for(size_t i=0; i<SAMPLES_PER_PIXEL; i++) {
            px_rays.emplace_back(MakeCameraRay(x_coor, y_coor));
        }
        return Pixel{std::move(px_rays)};
    }

    std::vector<Pixel> MakeAllPixels() const {
        std::vector<Pixel> all_pixels;
        all_pixels.reserve(static_cast<size_t>(width_*height_));
        for(double h=0;  h<height_; h++) {
            for(double w=0; w<width_; w++) {
                all_pixels.push_back(CreatePixel(w, h));
            }
        }
        return all_pixels;
    }


    double GetWidth() const {return width_;}
    double GetHeight() const {return height_;}
    double GetDistToPlane() const {return dist_to_plane_;}

};

inline size_t Camera::SAMPLES_PER_PIXEL = 10;

#endif  //CAMERA_H

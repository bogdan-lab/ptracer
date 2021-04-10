#ifndef PIXEL_H
#define PIXEL_H

#include "Ray.h"
#include "Color.h"
#include "Scene.h"
#include "Renderer.h"

#include <cstdlib>
#include <vector>
#include <random>


class Pixel {
private:
    std::vector<Ray> in_rays_;
    Color color_;
public:
    Pixel() = delete;

    explicit Pixel(const std::vector<Ray>& rays)
        : in_rays_(rays), color_(0,0,0) {}

    explicit Pixel(std::vector<Ray>&& rays)
        : in_rays_(std::move(rays)), color_(0,0,0) {}


    void TracePixel(const Scene& universe);

    const Color& GetPixelColor() const {return color_;}

};




#endif //PIXEL_H

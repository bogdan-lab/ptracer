#ifndef PIXEL_H
#define PIXEL_H

#include "Ray.h"
#include "Color.h"
#include "Scene.h"

#include <cstdlib>
#include <vector>
#include <random>


enum class BounceHitInfo {
    kUndefined = 0,
    kHitSource,
    kHitNothing,
    kHitObject
};

struct BounceRecord {
    BounceHitInfo hit_info_;
    std::optional<Color> hit_obj_color_;
};


class Pixel {
private:
    static size_t BOUNCE_LIMIT;
    std::vector<Ray> in_rays_;
    Color color_;
public:
    Pixel() = delete;

    explicit Pixel(const std::vector<Ray>& rays)
        : in_rays_(rays), color_(0,0,0) {}

    explicit Pixel(std::vector<Ray>&& rays)
        : in_rays_(std::move(rays)), color_(0,0,0) {}

    static Color GetAverageColor(const std::vector<Color> colors);
    static void SetBounceLimit(size_t g_lim) {BOUNCE_LIMIT = g_lim;}

    void TracePixel(const Scene& universe);
    Color RenderRay(const Ray& ray, const Scene& universe);
    BounceRecord MakeRayBounce(Ray& ray, const ObjectCollection& all_objects);


    const std::vector<Ray>& GetPixelRays() const {return in_rays_;}
    const Color& GetPixelColor() const {return color_;}
};


inline size_t Pixel::BOUNCE_LIMIT = 50;

#endif //PIXEL_H

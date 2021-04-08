#ifndef RENDERER_H
#define RENDERER_H


#include "Color.h"
#include "Ray.h"
#include "Scene.h"

#include <cstdlib>
#include <random>
#include <vector>
#include <optional>

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


class Renderer {
private:
    static size_t BOUNCE_LIMIT;
    std::mt19937 rnd_;
    std::vector<Color> bounce_colors_;

public:

    explicit Renderer(size_t seed) : rnd_(seed) {}

    static void SetBounceLimit(size_t g_num) {BOUNCE_LIMIT = g_num;}
    static Color GetAverageColor(const std::vector<Color> colors);

    Color RenderRay(const Ray& ray, const Scene& universe);

    BounceRecord MakeRayBounce(Ray& ray, const ObjectCollection& all_objects);


};



#endif //RENDERER_H

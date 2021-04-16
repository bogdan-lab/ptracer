﻿#ifndef RENDERER_H
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
    //TODO Current architecture does not allow me to set this parameter for my pixels. Move it to Camera class as accessible field.
    //Do we really need this class? its entire purpose is in BOUNCE LIMIT and other stuff can be putted into the pixel methods..
    static size_t BOUNCE_LIMIT;
    std::vector<Color> bounce_colors_;

public:

    Renderer() = default;

    static void SetBounceLimit(size_t g_num) {BOUNCE_LIMIT = g_num;}
    static Color GetAverageColor(const std::vector<Color> colors);

    Color RenderRay(const Ray& ray, const Scene& universe);

    BounceRecord MakeRayBounce(Ray& ray, const ObjectCollection& all_objects);


};

inline size_t Renderer::BOUNCE_LIMIT = 50;


#endif //RENDERER_H

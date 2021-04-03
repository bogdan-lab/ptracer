#ifndef RENDERER_H
#define RENDERER_H


#include "Color.h"
#include "Ray.h"
#include "Scene.h"

#include <cstdlib>
#include <random>
#include <vector>
#include <optional>


class Renderer {
private:
    static size_t BOUNCE_LIMIT = 10;
    std::mt19937 rnd_;
    std::vector<Color> bounce_colors_;

public:

    explicit Renderer(size_t seed) : rnd_(seed) {}

    static void SetBounceLimit(size_t g_num) {BOUNCE_LIMIT = g_num;}

    Color RenderRay(const Ray& ray, const Scene& universe) {
        bounce_colors_.reserve(BOUNCE_LIMIT);
        size_t curr_bounces = 0;
        Ray curr_ray = ray;
        std::optional<Color> curr_color = std::nullopt;
        const auto& all_objects = universe.GetObjects();
        while (curr_bounces < BOUNCE_LIMIT) {
            curr_bounces++;
            curr_color = MakeRayBounce(curr_ray, all_objects);
            if(!curr_color) {
                return Color{0,0,0};
            }
            bounce_colors_.push_back(*curr_color);
        }
        return GetAverageColor();
    }

    std::optional<Color> MakeRayBounce(Ray& ray, const ObjectCollection& all_objects) {
        std::optional<double> dist = std::nullopt;
        for (const auto& el : all_objects) {
            dist = el->GetClosesDist(ray);
            if(dist) {
                //reflect and return obtained color
                ray = el->Reflect(ray, dist);
                return el->GetColor();
            }
        }
        //ray has not meet any object
        return std::nullopt;
    }


    Color GetAverageColor() const {
        size_t r = g = b = 0;
        for (const auto& el : bounce_colors_) {
            r += el.red_;
            g += el.green_;
            b += el.blue_;
        }
        const size_t b_num = bounce_colors_.size();
        return {r/b_num, g/b_num, b/b_num};
    }

};



#endif //RENDERER_H

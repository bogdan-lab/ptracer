#ifndef PIXEL_H
#define PIXEL_H

#include <cstdlib>
#include <random>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Objects.h"
#include "Ray.h"
#include "Scene.h"

/** Struct contains information about material and color of an object which was
 * hit by ray*/
struct BounceRecord {
  Material hit_obj_mat_;
  Color hit_obj_color_;
};

namespace pixel {
/**
 * Method calculates trace of the given ray in the universe and returns
 * average color for this trace
 *
 * @param[in] bounce_limit - maximum number of reflextion that one ray can
 * make, before it is decided that ray has not meet light source
 */
Color RenderRay(const Ray& ray, const Scene& universe, size_t bounce_limit);
/**
 * Method checks if given ray hits anything in the universe.
 * if it does - then method will perform reflection.
 *
 * @return information about hit material and its color
 */
BounceRecord MakeRayBounce(Ray& ray, const ObjectCollection& all_objects);
/**
 * Traces each ray in the given collection and returns averaged color for all
 * these rays.
 */
Color TraceRays(const std::vector<Ray>& in_rays, const Scene& universe,
                size_t bounce_limit) {
  std::vector<Color> accum_colors;
  accum_colors.reserve(in_rays.size());
  for (const auto& ray : in_rays) {
    accum_colors.push_back(RenderRay(ray, universe, bounce_limit));
  }
  return Color::GetAverageColor(accum_colors);
}

}  // namespace pixel

#endif  // PIXEL_H

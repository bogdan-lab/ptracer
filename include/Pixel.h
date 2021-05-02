#ifndef PIXEL_H
#define PIXEL_H

#include <cstdlib>
#include <random>
#include <vector>

#include "Color.h"
#include "Ray.h"
#include "Scene.h"

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

 public:
  Pixel() = default;

  static Color GetAverageColor(const std::vector<Color>& colors);
  static void SetBounceLimit(size_t g_lim) { BOUNCE_LIMIT = g_lim; }
  static void TruncColorsInTrace(std::vector<Color>& cols);

  Color TraceRays(const std::vector<Ray>& in_rays, const Scene& universe) const;
  Color RenderRay(const Ray& ray, const Scene& universe) const;
  BounceRecord MakeRayBounce(Ray& ray,
                             const ObjectCollection& all_objects) const;
};

inline size_t Pixel::BOUNCE_LIMIT = 50;

#endif  // PIXEL_H

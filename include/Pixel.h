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

struct BounceRecord {
  Material hit_obj_mat_;
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

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

  static Color GetAverageColor(const std::vector<Color>& colors) {
    int64_t r = 0;
    int64_t g = 0;
    int64_t b = 0;
    int64_t col_num = 0;
    for (const auto& el : colors) {
      if (el.IsColor()) {
        r += el.red_;
        g += el.green_;
        b += el.blue_;
        col_num++;
      }
    }
    if (!col_num) return colors::kBlack;
    return {r / col_num, g / col_num, b / col_num};
  }

  static void SetBounceLimit(size_t g_lim) { BOUNCE_LIMIT = g_lim; }

  static void TruncColorsInTrace(std::vector<Color>& cols) {
    // Light we see is reflection of the light from source.
    // if in light in source red == 0 there could be no red in reflection!
    // The same stays for the reflected light
    // Last note in colors is expected to be light source color -> trunc all
    // colors according to it
    if (cols.empty()) return;
    auto it_source = cols.rbegin();
    for (auto it = std::next(it_source); it != cols.rend(); it++) {
      it->TruncByColor(*it_source++);
    }
  }

  Color TraceRays(const std::vector<Ray>& in_rays,
                  const Scene& universe) const {
    std::vector<Color> accum_colors;
    accum_colors.reserve(in_rays.size());
    for (const auto& ray : in_rays) {
      accum_colors.push_back(RenderRay(ray, universe));
    }
    return GetAverageColor(accum_colors);
  }

  Color RenderRay(const Ray& ray, const Scene& universe) const;

  BounceRecord MakeRayBounce(Ray& ray,
                             const ObjectCollection& all_objects) const;
};

inline size_t Pixel::BOUNCE_LIMIT = 50;

#endif  // PIXEL_H

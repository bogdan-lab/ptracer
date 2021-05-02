#include "Pixel.h"

Color Pixel::TraceRays(const std::vector<Ray>& in_rays,
                       const Scene& universe) const {
  std::vector<Color> accum_colors;
  accum_colors.reserve(in_rays.size());
  for (const auto& ray : in_rays) {
    accum_colors.push_back(RenderRay(ray, universe));
  }
  return GetAverageColor(accum_colors);
}

void Pixel::TruncColorsInTrace(std::vector<Color>& cols) {
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

Color Pixel::GetAverageColor(const std::vector<Color>& colors) {
  int64_t r = 0;
  int64_t g = 0;
  int64_t b = 0;
  for (const auto& el : colors) {
    r += el.red_;
    g += el.green_;
    b += el.blue_;
  }
  const int64_t b_num = static_cast<int64_t>(colors.size());
  return {r / b_num, g / b_num, b / b_num};
}

Color Pixel::RenderRay(const Ray& ray, const Scene& universe) const {
  std::vector<Color> bounce_colors;
  bounce_colors.reserve(BOUNCE_LIMIT);
  size_t curr_bounces = 0;
  Ray curr_ray = ray;
  const auto& all_objects = universe.GetObjects();
  BounceRecord bc_rec;
  while (curr_bounces < BOUNCE_LIMIT - 1) {
    // should take into account all trace ONLY if eventually it heats light
    // source! so check that last hit separately
    curr_bounces++;
    bc_rec = MakeRayBounce(curr_ray, all_objects);
    switch (bc_rec.hit_info_) {
      case BounceHitInfo::kHitNothing:
        return Color{0, 0, 0};  // TODO Maybe set it somewhere as "shade color"?
                                // or "backgrounde color"
      case BounceHitInfo::kHitSource:
        bounce_colors.push_back(*bc_rec.hit_obj_color_);
        TruncColorsInTrace(bounce_colors);
        return GetAverageColor(bounce_colors);
      case BounceHitInfo::kHitObject:
        bounce_colors.push_back(*bc_rec.hit_obj_color_);
        break;
      default:
        exit(1);
    }
  }
  bc_rec = MakeRayBounce(curr_ray, all_objects);
  if (BounceHitInfo::kHitSource != bc_rec.hit_info_) {
    return Color{0, 0, 0};
  }
  bounce_colors.push_back(*bc_rec.hit_obj_color_);
  TruncColorsInTrace(bounce_colors);
  return GetAverageColor(bounce_colors);
}

BounceRecord Pixel::MakeRayBounce(Ray& ray,
                                  const ObjectCollection& all_objects) const {
  std::optional<double> dist = std::nullopt;
  size_t obj_idx = 0;
  double min_dist = std::numeric_limits<double>::max();
  for (size_t i = 0; i < all_objects.size(); i++) {
    const auto& cur_obj = all_objects[i];
    dist = cur_obj->GetClosesDist(ray);
    if (dist && dist > cur_obj->GetHitPrecision() && min_dist > *dist) {
      min_dist = *dist;
      obj_idx = i;
    }
  }
  if (min_dist == std::numeric_limits<double>::max()) {
    // hit nothing
    return {BounceHitInfo::kHitNothing, std::nullopt};
  }
  const auto& obj_color = all_objects[obj_idx]->GetColor();
  if (all_objects[obj_idx]->GetMaterial() == Material::kLightSource) {
    return {BounceHitInfo::kHitSource, obj_color};
  }
  all_objects[obj_idx]->Reflect(ray, min_dist);
  return {BounceHitInfo::kHitObject, obj_color};
}

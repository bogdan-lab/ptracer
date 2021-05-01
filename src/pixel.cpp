#include "Pixel.h"

Color Pixel::TraceRays(const std::vector<Ray>& in_rays,
                       const Scene& universe) const {
  std::vector<Color> accum_colors_;
  accum_colors_.reserve(in_rays.size());
  for (const auto& ray : in_rays) {
    accum_colors_.push_back(RenderRay(ray, universe));
  }
  return GetAverageColor(accum_colors_);
}

Color Pixel::GetAverageColor(const std::vector<Color> colors) {
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

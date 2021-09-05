#include "Pixel.h"

Color Pixel::RenderRay(const Ray& ray, const Scene& universe) const {
  std::vector<Color> bounce_colors;
  bounce_colors.reserve(BOUNCE_LIMIT);
  Ray curr_ray = ray;
  const auto& all_objects = universe.GetObjects();
  BounceRecord bc_rec;
  for (size_t curr_bnc = 0; curr_bnc < BOUNCE_LIMIT - 1; curr_bnc++) {
    // should take into account all trace ONLY if eventually it hits light
    // source! so check that last hit separately
    bc_rec = MakeRayBounce(curr_ray, all_objects);
    switch (bc_rec.hit_obj_mat_) {
      case Material::kNoMaterial:
        return Camera::GetEmptyColor();
      case Material::kLightSource:
        bounce_colors.push_back(*bc_rec.hit_obj_color_);
        TruncColorsInTrace(bounce_colors);
        return GetAverageColor(bounce_colors);
      case Material::kReflective:
        bounce_colors.push_back(*bc_rec.hit_obj_color_);
        break;
      default:
        throw std::logic_error("Met unknown material in RenderRay()");
    }
  }
  bc_rec = MakeRayBounce(curr_ray, all_objects);
  if (bc_rec.hit_obj_mat_ != Material::kLightSource) {
    return Camera::GetEmptyColor();
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
    return {Material::kNoMaterial, std::nullopt};
  }
  const auto& hit_obj = all_objects[obj_idx];
  switch (hit_obj->GetMaterial()) {
    case Material::kLightSource:
      break;
    case Material::kReflective: {
      bool success = hit_obj->TryReflect(ray, min_dist);
      if (!success) return {Material::kNoMaterial, std::nullopt};
      break;
    }
    default:
      throw std::logic_error("Met unknown material in MakeRayBounce");
  }
  return {hit_obj->GetMaterial(), hit_obj->GetColor()};
}

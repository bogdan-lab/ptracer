#include "Pixel.h"

Color pixel::RenderRay(const Ray& ray,
                       const std::vector<const Object*>& universe,
                       size_t bounce_limit) {
  if (bounce_limit == 0) return colors::kBlack;
  std::vector<Color> bounce_colors;
  bounce_colors.reserve(bounce_limit);
  Ray curr_ray = ray;
  BounceRecord bc_rec;
  for (size_t curr_bnc = 0; curr_bnc < bounce_limit - 1; curr_bnc++) {
    // should take into account all trace ONLY if eventually it hits light
    // source! so check that last hit separately
    bc_rec = MakeRayBounce(curr_ray, universe);
    switch (bc_rec.hit_obj_mat_) {
      case Material::kNoMaterial:
        return colors::kBlack;
      case Material::kLightSource:
        bounce_colors.push_back(bc_rec.hit_obj_color_);
        Color::TruncColorsInTrace(bounce_colors);
        return Color::GetAverageColor(bounce_colors);
      case Material::kReflective:
        bounce_colors.push_back(bc_rec.hit_obj_color_);
        break;
      default:
        throw std::logic_error("Met unknown material in RenderRay()");
    }
  }
  bc_rec = MakeRayBounce(curr_ray, universe);
  if (bc_rec.hit_obj_mat_ != Material::kLightSource) {
    return colors::kBlack;
  }
  bounce_colors.push_back(bc_rec.hit_obj_color_);
  Color::TruncColorsInTrace(bounce_colors);
  return Color::GetAverageColor(bounce_colors);
}

BounceRecord pixel::MakeRayBounce(
    Ray& ray, const std::vector<const Object*>& all_objects) {
  std::optional<double> dist = std::nullopt;
  size_t obj_idx = 0;
  double min_dist = std::numeric_limits<double>::max();
  for (size_t i = 0; i < all_objects.size(); i++) {
    const auto& cur_obj = all_objects[i];
    dist = cur_obj->GetClosesDist(ray);
    if (dist && min_dist > *dist) {
      min_dist = *dist;
      obj_idx = i;
    }
  }
  if (min_dist == std::numeric_limits<double>::max()) {
    // hit nothing
    return {Material::kNoMaterial, colors::kNoColor};
  }
  const Object* hit_obj = all_objects[obj_idx];
  switch (hit_obj->GetMaterial()) {
    case Material::kLightSource:
      break;
    case Material::kReflective: {
      bool success = hit_obj->TryReflect(ray, min_dist);
      if (!success) return {Material::kNoMaterial, colors::kNoColor};
      break;
    }
    default:
      throw std::logic_error("Met unknown material in MakeRayBounce");
  }
  return {hit_obj->GetMaterial(), hit_obj->GetColor()};
}

std::vector<Ray> pixel::CreateRays(const Tile& tile, const GeoVec& ray_start,
                                   std::mt19937& rnd, int ray_num) {
  GeoVec to_top_left(ray_start, tile.top_left);
  std::vector<Ray> result;
  result.reserve(ray_num);
  std::uniform_real_distribution<double> dist{0.0, 1.0};
  while (ray_num--) {
    result.emplace_back(ray_start, to_top_left + dist(rnd) * tile.width_vec +
                                       dist(rnd) * tile.height_vec);
  }
  return result;
}

std::vector<pixel::Tile> pixel::CreateTiles(const GeoVec& top_left_point,
                                            const GeoVec& top_right_point,
                                            const GeoVec& bot_left_point,
                                            int width_in_pixel,
                                            int height_in_pixel) {
  auto width_vec = GeoVec(top_left_point, top_right_point) / width_in_pixel;
  auto height_vec = GeoVec(top_left_point, bot_left_point) / height_in_pixel;
  std::vector<Tile> result;
  result.reserve(width_in_pixel * height_in_pixel);
  for (int h = 0; h < height_in_pixel; ++h) {
    for (int w = 0; w < width_in_pixel; ++w) {
      result.push_back({top_left_point + w * width_vec + h * height_vec,
                        width_vec, height_vec});
    }
  }
  return result;
}

GeoVec pixel::CreateViewerPoint(const CameraSettings& cs) {
  GeoVec w_vec{cs.screen_top_left_coor, cs.screen_top_right_coor};
  GeoVec h_vec{cs.screen_top_left_coor, cs.screen_bot_left_coor};
  GeoVec norm = h_vec.Cross(w_vec).Norm();
  GeoVec mid_point = 0.5 * (cs.screen_top_right_coor + cs.screen_bot_left_coor);
  return mid_point + norm * cs.distance_to_screen;
}

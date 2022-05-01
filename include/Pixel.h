#ifndef PIXEL_H
#define PIXEL_H

#include <cstdlib>
#include <random>
#include <vector>

#include "Color.h"
#include "Config.h"
#include "Objects.h"
#include "Ray.h"

/** Struct contains information about material and color of an object which was
 * hit by ray*/
struct BounceRecord {
  Material hit_obj_mat_;
  Color hit_obj_color_;
};
namespace pixel {
/**Each Tile instance describes phisical coordinates of certain pixel.*/
struct Tile {
  GeoVec top_left;
  GeoVec width_vec;
  GeoVec height_vec;
};
/**Creates point from which all screen rays will be emitted.*/
GeoVec CreateViewerPoint(const CameraSettings& cs);
/**
 * Creates collecciton of rays pointing from the ray_start to a random point
 * in the tile
 */
std::vector<Ray> CreateRays(const Tile& tile, const GeoVec& ray_start,
                            std::mt19937& rnd, int ray_num);
/**
 * Creates tiles for the given plane. Note that tiles are listed frol left to
 * right, from up to bottom
 */
std::vector<Tile> CreateTiles(const GeoVec& top_left_point,
                              const GeoVec& top_right_point,
                              const GeoVec& bot_left_point, int width_in_pixel,
                              int height_in_pixel);
/**
 * Method calculates trace of the given ray in the universe and returns
 * average color for this trace
 *
 * @param[in] bounce_limit - maximum number of reflextion that one ray can
 * make, before it is decided that ray has not meet light source
 */
Color RenderRay(const Ray& ray, const std::vector<const Object*>& universe,
                size_t bounce_limit);
/**
 * Method checks if given ray hits anything in the universe.
 * if it does - then method will perform reflection.
 *
 * @return information about hit material and its color
 */
BounceRecord MakeRayBounce(Ray& ray,
                           const std::vector<const Object*>& all_objects);
/**
 * Traces each ray in the given collection and returns averaged color for all
 * these rays.
 */
inline Color TraceRays(const std::vector<Ray>& in_rays,
                       const std::vector<const Object*>& universe,
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

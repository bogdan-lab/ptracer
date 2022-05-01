#include <fstream>
#include <memory>
#include <utility>
#include <vector>

#include "GeoVec.h"
#include "Objects.h"
#include "Pixel.h"
#include "PngWriter.h"
#include "Ray.h"

int main() {
  Config cfg("simple_scene.json");
  if (!cfg.GetCameraSettings() || !cfg.GetGeneralSettings() ||
      cfg.GetObjects().empty()) {
    std::cout << "Config was not parsed correctly\n";
    return 0;
  }
  GeneralSettings cfg_general = cfg.GetGeneralSettings().value();
  CameraSettings cfg_camera = cfg.GetCameraSettings().value();
  std::vector<pixel::Tile> tiles = pixel::CreateTiles(
      cfg_camera.screen_top_left_coor, cfg_camera.screen_top_right_coor,
      cfg_camera.screen_bot_left_coor, cfg_general.pic_width_in_pixel,
      cfg_general.pic_height_in_pixel);
  GeoVec viewer = pixel::CreateViewerPoint(cfg_camera);
  std::vector<Color> col_vec;
  col_vec.reserve(tiles.size());
  std::vector<const Object*> objects = cfg.GetObjects();
  std::mt19937 rnd{std::time(nullptr)};
  size_t count = 0;
  for (const auto& tile : tiles) {
    count++;
    col_vec.push_back(pixel::TraceRays(
        pixel::CreateRays(tile, viewer, rnd, cfg_general.sample_per_pixel),
        objects, cfg_general.max_bounce_number));
    if ((10ULL * count) % tiles.size() == 0ULL)
      std::cerr << 100.0 * count / tiles.size() << "%\n";
  }

  PngWriter pw{cfg_general.out_file_name.c_str(),
               cfg_general.pic_width_in_pixel, cfg_general.pic_height_in_pixel};
  if (!pw.IsOk()) {
    throw std::runtime_error("Unable to save png file");
  }
  pw.SaveVectorToImage(col_vec);
  return 0;
}

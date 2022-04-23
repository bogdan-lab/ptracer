#ifndef PATH_TRACER_CONFIG_H
#define PATH_TRACER_CONFIG_H

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <vector>

#include "Color.h"
#include "GeoVec.h"
#include "Objects.h"

struct CameraSettings {
  GeoVec screen_top_left_coor;
  GeoVec screen_top_right_coor;
  GeoVec screen_bot_left_coor;
  double distance_to_screen = 0.0;
};

struct GeneralSettings {
  int sample_per_pixel = 1;
  int max_bounce_number = 1;
  int pic_width_in_pixel = 1;
  int pic_height_in_pixel = 1;
  std::string out_file_name;
};

class Config {
 public:
  Config(const std::string& file_name);
  /**
   * @brief Returns parsed camera settings. If the returned value is not empty,
   * than and parameters are set correctly and there is no need to make some
   * additional checks.
   */
  const std::optional<CameraSettings>& GetCameraSettings() const {
    return camera_settings_;
  }
  /**
   * @brief Returns observer pointers, which will become dangling if the
   * corresponding config instance will be destroyed.
   */
  std::vector<const Object*> GetObjectSettings() const {
    std::vector<const Object*> result;
    result.reserve(objects_.size());
    for (const auto& el : objects_) {
      result.push_back(el.get());
    }
    return result;
  }
  /**
   * @brief Returns parsed general settings. If the returned value is not empty,
   * than and parameters are set correctly and there is no need to make some
   * additional checks.
   */
  const std::optional<GeneralSettings>& GetGeneralSettings() const {
    return general_settings_;
  }

 private:
  static std::optional<GeneralSettings> ParseGeneralSettings(
      const nlohmann::json& input);

  static std::optional<CameraSettings> ParseCameraSettings(
      const nlohmann::json& input);

  std::optional<CameraSettings> camera_settings_;
  std::optional<GeneralSettings> general_settings_;
  std::vector<std::unique_ptr<Object>> objects_;
};

#endif  // PATH_TRACER_CONFIG_H

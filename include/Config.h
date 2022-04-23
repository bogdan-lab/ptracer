#ifndef PATH_TRACER_CONFIG_H
#define PATH_TRACER_CONFIG_H

#include <fstream>
#include <optional>
#include <string_view>
#include <vector>

#include "Color.h"
#include "Objects.h"

struct CameraSettings {
  // screen_plane
  double distance_to_screen_;
};

struct GeneralSettings {
  size_t sample_per_pixel_ = 0;
  size_t max_bounce_number_ = 0;
  std::string out_file_name_;
};

class Config {
 public:
  Config(std::string_view file_name);

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

  const std::optional<GeneralSettings>& GetGeneralSettings() const {
    return general_settings_;
  }

 private:
  static std::optional<GeneralSettings> ParseGeneralSettings(
      std::ifstream& input);

  std::optional<CameraSettings> camera_settings_;
  std::optional<GeneralSettings> general_settings_;
  std::vector<std::unique_ptr<Object>> objects_;
};

#endif  // PATH_TRACER_CONFIG_H

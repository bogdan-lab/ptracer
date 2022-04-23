#include "Config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>

namespace {
template <typename NumberType>
bool ReadPositiveValue(const nlohmann::json& cfg, const std::string& name,
                       NumberType& out, NumberType def_value) {
  if (!cfg.contains(name)) {
    out = def_value;
    return true;
  }
  NumberType buff_val = cfg[name].get<NumberType>();
  if (buff_val <= 0) {
    std::cout << name << " must be positive!\n";
    return false;
  }
  out = buff_val;
  return true;
}

bool ReadPoint(const nlohmann::json& cfg, const std::string& name,
               GeoVec& out) {
  if (!cfg.contains(name)) {
    std::cout << "Cannot find " << name << " in the config file\n";
    return false;
  }
  if (cfg[name].size() != 3) {
    std::cout << "Point " << name
              << " should be defined by 3 coordinate values\n";
    return false;
  }
  double val = cfg[name].at(0).get<double>();
  if (std::isnan(val)) return false;
  out.x_ = val;
  val = cfg[name].at(1).get<double>();
  if (std::isnan(val)) return false;
  out.y_ = val;
  val = cfg[name].at(2).get<double>();
  if (std::isnan(val)) return false;
  out.z_ = val;
  return true;
}

}  // namespace

Config::Config(const std::string& file_name) {
  std::ifstream input{file_name};
  if (!input.is_open()) {
    std::cout << "Cannot open file " << file_name << '\n';
    return;
  }
  nlohmann::json cfg_json;
  input >> cfg_json;
  general_settings_ = ParseGeneralSettings(cfg_json["general"]);
  camera_settings_ = ParseCameraSettings(cfg_json["camera"]);
}

std::optional<GeneralSettings> Config::ParseGeneralSettings(
    const nlohmann::json& input) {
  GeneralSettings result;
  if (!ReadPositiveValue(input, "max_bounce_number", result.max_bounce_number,
                         1000)) {
    return std::nullopt;
  }
  if (!ReadPositiveValue(input, "number_of_samples_per_pixel",
                         result.sample_per_pixel, 500)) {
    return std::nullopt;
  }
  if (!ReadPositiveValue(input, "pic_width_in_pixel", result.pic_width_in_pixel,
                         600)) {
    return std::nullopt;
  }
  if (!ReadPositiveValue(input, "pic_height_in_pixel",
                         result.pic_height_in_pixel, 400)) {
    return std::nullopt;
  }
  result.out_file_name = input.contains("output_file")
                             ? input["output_file"].get<std::string>()
                             : "path_tracer_output.png";
  return result;
}

std::optional<CameraSettings> Config::ParseCameraSettings(
    const nlohmann::json& input) {
  CameraSettings result;
  if (!ReadPositiveValue(input, "distance_to_screen", result.distance_to_screen,
                         500.0)) {
    return std::nullopt;
  }
  if (!ReadPoint(input, "screen_top_left_coordinate",
                 result.screen_top_left_coor)) {
    return std::nullopt;
  }
  if (!ReadPoint(input, "screen_top_right_coordinate",
                 result.screen_top_right_coor)) {
    return std::nullopt;
  }
  if (!ReadPoint(input, "screen_bot_left_coordinate",
                 result.screen_bot_left_coor)) {
    return std::nullopt;
  }
  return result;
}

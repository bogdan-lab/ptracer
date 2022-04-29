#include "Config.h"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <vector>

namespace {
template <typename NumberType>
bool ReadPositiveValue(const nlohmann::json& cfg, const std::string& name,
                       NumberType& out) {
  if (!cfg.contains(name)) {
    return false;
  }
  NumberType buff_val = cfg[name].get<NumberType>();
  if (buff_val <= 0) {
    std::cout << name << " must be positive!\n";
    return false;
  }
  out = buff_val;
  return true;
}

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

bool ReadPoint(const nlohmann::json& point_arr, GeoVec& out) {
  if (point_arr.size() != 3) {
    return false;
  }
  double val = point_arr.at(0).get<double>();
  if (std::isnan(val)) return false;
  out.x_ = val;
  val = point_arr.at(1).get<double>();
  if (std::isnan(val)) return false;
  out.y_ = val;
  val = point_arr.at(2).get<double>();
  if (std::isnan(val)) return false;
  out.z_ = val;
  return true;
}

bool OpenJSONFile(const std::string& file_name, nlohmann::json& json) {
  std::ifstream input{file_name};
  if (!input.is_open()) {
    return false;
  }
  input >> json;
  return true;
}

}  // namespace

Config::Config(const std::string& file_name) {
  nlohmann::json cfg_json;
  if (!OpenJSONFile(file_name, cfg_json)) {
    std::cout << "Cannot open file " << file_name << '\n';
    return;
  }
  general_settings_ = ParseGeneralSettings(cfg_json["general"]);
  camera_settings_ = ParseCameraSettings(cfg_json["camera"]);
  objects_ = ParseObjects(cfg_json["objects"]);
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
  if (!input.contains("screen_top_left_coordinate") ||
      !ReadPoint(input["screen_top_left_coordinate"],
                 result.screen_top_left_coor)) {
    return std::nullopt;
  }
  if (!input.contains("screen_top_right_coordinate") ||
      !ReadPoint(input["screen_top_right_coordinate"],
                 result.screen_top_right_coor)) {
    return std::nullopt;
  }
  if (!input.contains("screen_bot_left_coordinate") ||
      !ReadPoint(input["screen_bot_left_coordinate"],
                 result.screen_bot_left_coor)) {
    return std::nullopt;
  }
  return result;
}

namespace {

std::optional<Color> StringToColor(std::string_view str) {
  static std::vector<std::string> names{
      "BLACK", "WHITE",     "RED",    "GREEN",      "BLUE",
      "GREY",  "ORANGE",    "YELLOW", "LIGHTGREEN", "PALEGREEN",
      "CAYAN", "LIGHTBLUE", "VIOLET", "PURPLE"};
  static std::vector<Color> colors{
      colors::kBlack,     colors::kWhite,  colors::kRed,
      colors::kGreen,     colors::kBlue,   colors::kGrey,
      colors::kOrange,    colors::kYellow, colors::kLightGreen,
      colors::kPaleGreen, colors::kCayan,  colors::kLightBlue,
      colors::kViolet,    colors::kPurple};
  std::string upper(str);
  std::transform(
      upper.begin(), upper.end(), upper.begin(),
      [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
  auto it = std::find(names.begin(), names.end(), upper);
  if (it == names.end()) return std::nullopt;
  return *(colors.begin() + (it - names.begin()));
}

bool ReadColor(const nlohmann::json& cfg, const std::string& name, Color& out,
               Color def_color) {
  if (!cfg.contains(name)) {
    out = def_color;
    return true;
  }
  const auto& node = cfg[name];
  if (node.is_array()) {
    if (node.size() != 3) return false;
    int val = node.at(0).get<int>();
    if (val < 0) return false;
    out.red_ = static_cast<uint8_t>(std::clamp(val, 0, 255));
    val = node.at(1).get<int>();
    if (val < 0) return false;
    out.green_ = static_cast<uint8_t>(std::clamp(val, 0, 255));
    val = node.at(2).get<int>();
    if (val < 0) return false;
    out.blue_ = static_cast<uint8_t>(std::clamp(val, 0, 255));
    return true;
  } else if (node.is_string()) {
    std::optional<Color> buff = StringToColor(node.get<std::string>());
    if (!buff) return false;
    out = buff.value();
    return true;
  } else {
    return false;
  }
}

std::unique_ptr<Object> ReadSphere(const nlohmann::json& cfg) {
  GeoVec center;
  if (!cfg.contains("center") || !ReadPoint(cfg["center"], center)) {
    return nullptr;
  }
  double buff = std::numeric_limits<double>::quiet_NaN();
  if (!ReadPositiveValue(cfg, "radius", buff)) {
    return nullptr;
  }

  auto result = std::make_unique<Sphere>(center, buff);
  if (!ReadPositiveValue(cfg, "reflection", buff, 0.75)) {
    return nullptr;
  }
  result->SetReflectionCoef(std::clamp(buff, 0.0, 1.0));

  if (!ReadPositiveValue(cfg, "polishness", buff, 0.9)) {
    return nullptr;
  }
  result->SetPolishness(std::clamp(buff, 0.0, 1.0));

  Color color;
  if (!ReadColor(cfg, "color", color, colors::kGreen)) {
    return nullptr;
  }
  result->SetColor(color);

  if (!cfg.contains("is_light_source")) return nullptr;
  Material mat = cfg["is_light_source"].get<bool>() ? Material::kLightSource
                                                    : Material::kReflective;
  result->SetMaterial(mat);
  return result;
}

bool ReadPointVector(const nlohmann::json& node, std::vector<GeoVec>& out) {
  out.clear();
  out.reserve(node.size());
  GeoVec point;
  for (const auto& el : node) {
    if (!ReadPoint(el, point)) return false;
    out.push_back(point);
  }
  return true;
}

bool ReadFacesVector(const nlohmann::json& node,
                     std::vector<std::tuple<int, int, int>>& out) {
  out.reserve(node.size());
  for (const auto& el : node) {
    if (el.size() != 3) return false;
    int i1 = el.at(0).get<int>();
    int i2 = el.at(1).get<int>();
    int i3 = el.at(2).get<int>();
    if (i1 < 0 || i2 < 0 || i3 < 0) return false;
    out.push_back(std::make_tuple(i1, i2, i3));
  }
  return true;
}

bool ReadTriangles(const nlohmann::json& node,
                   std::vector<std::unique_ptr<Object>>& out) {
  Color color;
  if (!ReadColor(node, "color", color, colors::kGreen)) {
    return false;
  }
  if (!node.contains("is_light_source")) return false;
  Material mat = node["is_light_source"].get<bool>() ? Material::kLightSource
                                                     : Material::kReflective;
  double reflection = std::numeric_limits<double>::quiet_NaN();
  if (!ReadPositiveValue(node, "reflection", reflection, 0.75)) {
    return false;
  }
  reflection = std::clamp(reflection, 0.0, 1.0);

  double polishness = std::numeric_limits<double>::quiet_NaN();
  if (!ReadPositiveValue(node, "polishness", polishness, 0.9)) {
    return false;
  }
  polishness = std::clamp(polishness, 0.0, 1.0);

  std::vector<GeoVec> points;
  std::vector<std::tuple<int, int, int>> faces;
  if (node.contains("description")) {
    // points and faces are described in separate file
    nlohmann::json prop_json;

    if (!OpenJSONFile(node["description"], prop_json)) return false;
    if (!prop_json.contains("points") ||
        !ReadPointVector(prop_json["points"], points)) {
      return false;
    }
    if (!prop_json.contains("faces") ||
        !ReadFacesVector(prop_json["faces"], faces)) {
      return false;
    }

  } else {
    if (!node.contains("points") || !ReadPointVector(node["points"], points)) {
      return false;
    }
    if (!node.contains("faces") || !ReadFacesVector(node["faces"], faces)) {
      return false;
    }
  }
  size_t pts_size = points.size();
  if (std::any_of(faces.begin(), faces.end(), [pts_size](const auto& f) {
        return std::get<0>(f) >= pts_size || std::get<1>(f) >= pts_size ||
               std::get<2>(f) >= pts_size;
      })) {
    return false;
  }

  out.reserve(faces.size());
  for (const auto& f : faces) {
    auto tri = std::make_unique<Triangle>(
        points[std::get<0>(f)], points[std::get<1>(f)], points[std::get<2>(f)]);
    tri->SetColor(color)
        .SetMaterial(mat)
        .SetPolishness(polishness)
        .SetReflectionCoef(reflection);
    out.push_back(std::move(tri));
  }
  return true;
}

}  // namespace

std::vector<std::unique_ptr<Object>> Config::ParseObjects(
    const nlohmann::json& input) {
  std::vector<std::unique_ptr<Object>> result;
  int count = 0;
  std::string type;
  for (auto node : input) {
    count++;
    if (!node.contains("type")) {
      std::cout << "Object with index " << count - 1 << " has no type!\n";
      continue;
    }
    type = node["type"];
    std::transform(
        type.begin(), type.end(), type.begin(),
        [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    if (type == "SPHERE") {
      std::unique_ptr<Object> sphere = ReadSphere(node);
      if (!sphere) {
        std::cout << "Bad object with index " << count - 1 << '\n';
        continue;
      }
      result.push_back(std::move(sphere));
    } else if (type == "TRIANGLES") {
      std::vector<std::unique_ptr<Object>> triangles;
      if (!ReadTriangles(node, triangles)) {
        std::cout << "Bad object with index " << count - 1 << '\n';
        continue;
      }
      for (auto& el : triangles) {
        result.push_back(std::move(el));
      }
    } else {
      std::cout << "Unknown type = " << node["type"] << '\n';
    }
  }
  return result;
}

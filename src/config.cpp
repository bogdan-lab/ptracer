#include "Config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>

namespace {
bool ReadAndValidateSizeT(const nlohmann::json& cfg, const std::string& name,
                          size_t& out, size_t def_value) {
  if (!cfg.contains(name)) {
    out = def_value;
    return true;
  }
  int64_t buff_int = cfg[name].get<int64_t>();
  if (buff_int <= 0) {
    std::cout << name << " must be positive!\n";
    return false;
  }
  out = static_cast<size_t>(buff_int);
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
}

std::optional<GeneralSettings> Config::ParseGeneralSettings(
    const nlohmann::json& input) {
  GeneralSettings result;
  if (!ReadAndValidateSizeT(input, "max_bounce_number",
                            result.max_bounce_number, 1000)) {
    return std::nullopt;
  }
  if (!ReadAndValidateSizeT(input, "number_of_samples_per_pixel",
                            result.sample_per_pixel, 500)) {
    return std::nullopt;
  }
  result.out_file_name = input.contains("output_file")
                             ? input["output_file"].get<std::string>()
                             : "path_tracer_output.png";
  return result;
}

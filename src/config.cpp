#include "Config.h"

#include <fstream>
#include <optional>
#include <string_view>

Config::Config(std::string_view file_name) {
  std::ifstream input{std::string(file_name)};
  if (!input.is_open()) {
    std::cout << "Cannot open file " << file_name << '\n';
    return;
  }
  general_settings_ = ParseGeneralSettings(input);
}

std::optional<GeneralSettings> Config::ParseGeneralSettings(
    std::ifstream& input) {
  return std::nullopt;
}

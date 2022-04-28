﻿#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "Config.h"

using ::testing::Each;
using ::testing::NotNull;

namespace {

class RAIIConfigFile {
 public:
  static std::unique_ptr<RAIIConfigFile> CreateFile(
      const std::string& file_name, std::string_view content) {
    std::ofstream out{file_name};
    if (!out.is_open()) {
      return nullptr;
    }
    out << content;
    return std::unique_ptr<RAIIConfigFile>(new RAIIConfigFile(file_name));
  }

  ~RAIIConfigFile() {
    if (!created_file_.empty()) {
      std::filesystem::remove(created_file_);
    }
  }

 private:
  RAIIConfigFile(const std::string& file_name) : created_file_(file_name) {}

  std::string created_file_;
};

}  // namespace

TEST(ConfigTest, ParsingGeneralOptioins1) {
  // Good case
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["general"] = {{"max_bounce_number", 150},
                    {"number_of_samples_per_pixel", 345},
                    {"pic_width_in_pixel", 1200},
                    {"pic_height_in_pixel", 798},
                    {"output_file", "test_output.png"}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<GeneralSettings>& res = test.GetGeneralSettings();
  ASSERT_TRUE(res);
  EXPECT_EQ(res->max_bounce_number, 150);
  EXPECT_EQ(res->sample_per_pixel, 345);
  EXPECT_EQ(res->pic_width_in_pixel, 1200);
  EXPECT_EQ(res->pic_height_in_pixel, 798);
  EXPECT_EQ(res->out_file_name, "test_output.png");
}

TEST(ConfigTest, ParsingGeneralOptions2) {
  // Case with invalid data
  {  // invalid max bounce number
    std::string file_name = "test_config.json";
    nlohmann::json cfg;
    cfg["general"] = {{"max_bounce_number", -150},
                      {"number_of_samples_per_pixel", 345},
                      {"output_file", "test_output.png"}};
    std::unique_ptr<RAIIConfigFile> config_file =
        RAIIConfigFile::CreateFile(file_name, cfg.dump());
    ASSERT_TRUE(config_file);
    Config test{file_name};
    const std::optional<GeneralSettings>& res = test.GetGeneralSettings();
    EXPECT_FALSE(res);
  }
  {
    // invalid max bounce number
    std::string file_name = "test_config.json";
    nlohmann::json cfg;
    cfg["general"] = {{"max_bounce_number", 150},
                      {"number_of_samples_per_pixel", 0},
                      {"output_file", "test_output.png"}};
    std::unique_ptr<RAIIConfigFile> config_file =
        RAIIConfigFile::CreateFile(file_name, cfg.dump());
    ASSERT_TRUE(config_file);
    Config test{file_name};
    const std::optional<GeneralSettings>& res = test.GetGeneralSettings();
    EXPECT_FALSE(res);
  }
}

TEST(ConfigTest, ParsingGeneralOptioins3) {
  // If some parameters are not set they should use default value
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["general"] = {};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<GeneralSettings>& res = test.GetGeneralSettings();
  ASSERT_TRUE(res);
  EXPECT_EQ(res->max_bounce_number, 1000);
  EXPECT_EQ(res->sample_per_pixel, 500);
  EXPECT_EQ(res->pic_width_in_pixel, 600);
  EXPECT_EQ(res->pic_height_in_pixel, 400);
  EXPECT_EQ(res->out_file_name, "path_tracer_output.png");
}

TEST(ConfigTest, ParsingCameraOption1) {
  // Good case
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["camera"] = {{"distance_to_screen", 42.5},
                   {"screen_top_left_coordinate", {1.0, 2.0, 3.0}},
                   {"screen_top_right_coordinate", {4.0, 5.0, 6.0}},
                   {"screen_bot_left_coordinate", {7.0, 8.0, 9.0}}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<CameraSettings>& res = test.GetCameraSettings();
  ASSERT_TRUE(res);
  EXPECT_DOUBLE_EQ(res->distance_to_screen, 42.5);
  EXPECT_DOUBLE_EQ(res->screen_top_left_coor.x_, 1.0);
  EXPECT_DOUBLE_EQ(res->screen_top_left_coor.y_, 2.0);
  EXPECT_DOUBLE_EQ(res->screen_top_left_coor.z_, 3.0);

  EXPECT_DOUBLE_EQ(res->screen_top_right_coor.x_, 4.0);
  EXPECT_DOUBLE_EQ(res->screen_top_right_coor.y_, 5.0);
  EXPECT_DOUBLE_EQ(res->screen_top_right_coor.z_, 6.0);

  EXPECT_DOUBLE_EQ(res->screen_bot_left_coor.x_, 7.0);
  EXPECT_DOUBLE_EQ(res->screen_bot_left_coor.y_, 8.0);
  EXPECT_DOUBLE_EQ(res->screen_bot_left_coor.z_, 9.0);
}

TEST(ConfigTest, ParsingCameraOption2) {
  // Wrong distance
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["camera"] = {{"distance_to_screen", -42.5},
                   {"screen_top_left_coordinate", {1.0, 2.0, 3.0}},
                   {"screen_top_right_coordinate", {4.0, 5.0, 6.0}},
                   {"screen_bot_left_coordinate", {7.0, 8.0, 9.0}}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<CameraSettings>& res = test.GetCameraSettings();
  EXPECT_FALSE(res);
}

TEST(ConfigTest, ParsingCameraOption3) {
  // One point is absent
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["camera"] = {{"distance_to_screen", 42.5},
                   {"screen_top_left_coordinate", {1.0, 2.0, 3.0}},
                   {"screen_bot_left_coordinate", {7.0, 8.0, 9.0}}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<CameraSettings>& res = test.GetCameraSettings();
  EXPECT_FALSE(res);
}

TEST(ConfigTest, ParsingCameraOption4) {
  // Wrong size of the point
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["camera"] = {{"distance_to_screen", -42.5},
                   {"screen_top_left_coordinate", {1.0, 2.0, 3.0}},
                   {"screen_top_right_coordinate", {4.0, 5.0, 6.0}},
                   {"screen_bot_left_coordinate", {7.0, 8.0}}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test{file_name};
  const std::optional<CameraSettings>& res = test.GetCameraSettings();
  EXPECT_FALSE(res);
}

TEST(ConfigTest, ParsingObjectOption1) {
  // Parsing only spheres
  std::string file_name = "test_config.json";
  nlohmann::json cfg;
  cfg["objects"] = {
      {{"color", {0, 0, 255}},  // TODO Want to parse string here too
       {"polishness", 0.1},
       {"reflection", 1.0},
       {"is_light_source", false},
       {"type", "sphere"},
       {"radius", 80.0},
       {"center", {1, 2, 3}}},
      {{"color", {100, 200, 50}},
       {"polishness", 0.5},
       {"reflection", 0.2},
       {"is_light_source", true},
       {"type", "sphere"},
       {"radius", 10.0},
       {"center", {5, 6, 7}}}};
  std::unique_ptr<RAIIConfigFile> config_file =
      RAIIConfigFile::CreateFile(file_name, cfg.dump());
  ASSERT_TRUE(config_file);
  Config test(file_name);
  std::vector<const Object*> res = test.GetObjects();
  ASSERT_EQ(res.size(), 2);
  ASSERT_THAT(res, Each(NotNull()));
  const Sphere* sph1 = dynamic_cast<const Sphere*>(res.front());
  ASSERT_THAT(sph1, NotNull());
  EXPECT_EQ(sph1->GetColor(), colors::kBlue);
  EXPECT_EQ(sph1->GetMaterial(), Material::kReflective);
  EXPECT_DOUBLE_EQ(sph1->GetPolishness(), 0.1);
  EXPECT_DOUBLE_EQ(sph1->GetReflectionCoefficient(), 1.0);
  EXPECT_DOUBLE_EQ(sph1->GetRadius(), 80.0);
  EXPECT_EQ(sph1->GetCenter(), GeoVec(1, 2, 3));

  const Sphere* sph2 = dynamic_cast<const Sphere*>(res.back());
  ASSERT_THAT(sph2, NotNull());
  EXPECT_EQ(sph2->GetColor(), Color(100, 200, 50));
  EXPECT_EQ(sph2->GetMaterial(), Material::kLightSource);
  EXPECT_DOUBLE_EQ(sph2->GetPolishness(), 0.5);
  EXPECT_DOUBLE_EQ(sph2->GetReflectionCoefficient(), 0.2);
  EXPECT_DOUBLE_EQ(sph2->GetRadius(), 10.0);
  EXPECT_EQ(sph2->GetCenter(), GeoVec(5, 6, 7));
}

// TODO add sphere test with bad data: reflection, polishness, color - out of
// bounds string values should be compared case insensitively
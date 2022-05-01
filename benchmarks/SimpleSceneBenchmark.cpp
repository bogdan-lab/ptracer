#include <memory>

#include "BenchmarkBase.h"
#include "Camera.h"
#include "Color.h"
#include "GeoVec.h"
#include "Objects.h"
#include "Pixel.h"
#include "PngWriter.h"
#include "Scene.h"
#include "benchmark_info.h"

class SimpleSceneBenchmark : public BenchmarkBase {
 private:
  Scene universe_;

 public:
  SimpleSceneBenchmark()
      : BenchmarkBase(BENCHMARK_DIR_PATH, "SimpleSceneBenchmark") {}

  void CreateUniverse() override {
    double width = 600;
    double height = 400;
    double depth = 400;
    double sph_radius = 80;
    GeoVec ftop_left{0, 0, 0};
    GeoVec ftop_right{width, 0, 0};
    GeoVec fbot_left{0, height, 0};
    GeoVec fbot_right{width, height, 0};

    GeoVec btop_left{0, 0, depth};
    GeoVec btop_right{width, 0, depth};
    GeoVec bbot_left{0, height, depth};
    GeoVec bbot_right{width, height, depth};

    // COMMON SPHERE
    ObjHolder sphere = std::make_unique<Sphere>(
        GeoVec{width / 2, height - sph_radius, depth / 2}, sph_radius);
    sphere->SetColor(colors::kBlue)
        .SetMaterial(Material::kReflective)
        .SetPolishness(0.1)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(sphere));
    // REFLECTIVE SPHERE
    sphere = std::make_unique<Sphere>(
        GeoVec{width / 2 - 2.1 * sph_radius, height - sph_radius, depth / 2},
        sph_radius);
    sphere->SetColor(colors::kNoColor)
        .SetMaterial(Material::kReflective)
        .SetPolishness(1.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(sphere));
    // ABSORBING SPHERE
    sphere = std::make_unique<Sphere>(
        GeoVec{width / 2 + 2.1 * sph_radius, height - sph_radius, depth / 2},
        sph_radius);
    sphere->SetColor(colors::kPurple)
        .SetMaterial(Material::kReflective)
        .SetPolishness(0.0)
        .SetReflectionCoef(0.75);
    universe_.AddObject(std::move(sphere));
    Material wall_material = Material::kReflective;
    // Creating walls
    // LEFT
    Color left_wall_color = colors::kRed;
    ObjHolder trian =
        std::make_unique<Triangle>(fbot_left, btop_left, ftop_left);
    trian->SetColor(left_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(fbot_left, bbot_left, btop_left);
    trian->SetColor(left_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    // RIGHT
    Color right_wall_color = colors::kGreen;
    trian = std::make_unique<Triangle>(fbot_right, ftop_right, btop_right);
    trian->SetColor(right_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(fbot_right, btop_right, bbot_right);
    trian->SetColor(right_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    // Bottom
    Color bot_wall_color = colors::kGrey;
    trian = std::make_unique<Triangle>(fbot_left, fbot_right, bbot_left);
    trian->SetColor(bot_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(fbot_right, bbot_right, bbot_left);
    trian->SetColor(bot_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    // TOP
    Color top_wall_color = colors::kWhite;
    trian = std::make_unique<Triangle>(ftop_right, ftop_left, btop_left);
    trian->SetColor(top_wall_color)
        .SetMaterial(Material::kLightSource)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(ftop_right, btop_left, btop_right);
    trian->SetColor(top_wall_color)
        .SetMaterial(Material::kLightSource)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    // BACK
    Color back_wall_color = colors::kCayan;
    trian = std::make_unique<Triangle>(bbot_left, btop_right, btop_left);
    trian->SetColor(back_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(bbot_left, bbot_right, btop_right);
    trian->SetColor(back_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(0.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    // FRONT
    Color front_wall_color = colors::kNoColor;
    trian = std::make_unique<Triangle>(fbot_left, ftop_left, ftop_right);
    trian->SetColor(front_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(1.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
    trian = std::make_unique<Triangle>(fbot_left, ftop_right, fbot_right);
    trian->SetColor(front_wall_color)
        .SetMaterial(wall_material)
        .SetPolishness(1.0)
        .SetReflectionCoef(1.0);
    universe_.AddObject(std::move(trian));
  }

  void MkPicture() override {
    Camera cam;
    Camera::SetSamplePerPixel(400);
    size_t px_num = cam.GetPxNum();
    std::vector<Color> col_vec;
    col_vec.reserve(px_num);
    // temporary fix just for compilateion!
    std::vector<const Object*> tmp;
    for (const auto& el : universe_.GetObjects()) {
      tmp.push_back(el.get());
    }

    for (size_t i = 0; i < px_num; i++) {
      col_vec.push_back(pixel::TraceRays(cam.GetPixelRays(i), tmp,
                                         /*bounce_limit=*/1000));
      if ((10 * i) % px_num == 0) std::cerr << 100.0 * i / px_num << "%\n";
    }

    std::string pic_name = GetPath() + GetName() + std::string(".png");
    PngWriter pw{pic_name.c_str(), cam.GetWidthInPx(), cam.GetHeightInPx()};
    if (!pw.IsOk()) {
      throw std::runtime_error("Unable to save png file");
    }
    pw.SaveVectorToImage(col_vec);
  }
};

int main() {
  SimpleSceneBenchmark sp;
  sp.SetGitBranch(CURRENT_GIT_BRANCH)
      .SetGitCommit(CURRENT_GIT_COMMIT)
      .SetCompilerInfo(COMPILER_INFO);
  sp.CreateUniverse();
  sp.MkPicture();
  return 0;
}

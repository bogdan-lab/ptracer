#include <fstream>
#include <memory>
#include <utility>
#include <vector>

#include "Camera.h"
#include "GeoVec.h"
#include "Objects.h"
#include "Pixel.h"
#include "Ray.h"
#include "Scene.h"

#define HIT_PRECISION 1e-9

Scene MakeSimpleRoomScene(double width, double height, double depth,
                          double src_radius) {
  GeoVec ftop_left{0, 0, 0};
  GeoVec ftop_right{width, 0, 0};
  GeoVec fbot_left{0, height, 0};
  GeoVec fbot_right{width, height, 0};

  GeoVec btop_left{0, 0, depth};
  GeoVec btop_right{width, 0, depth};
  GeoVec bbot_left{0, height, depth};
  GeoVec bbot_right{width, height, depth};

  Scene universe;

  ObjHolder source = std::make_unique<Sphere>(
      GeoVec{width / 2, height - src_radius, depth / 2}, src_radius);
  source->SetColor({0, 0, 255})
      .SetMaterial(Material::kCommon)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(source));
  Material wall_material = Material::kCommon;
  // Creating walls
  // LEFT
  Color left_wall_color{255, 0, 0};
  ObjHolder trian = std::make_unique<Triangle>(fbot_left, btop_left, ftop_left);
  trian->SetColor(left_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  trian = std::make_unique<Triangle>(fbot_left, bbot_left, btop_left);
  trian->SetColor(left_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  // RIGHT
  Color right_wall_color{0, 255, 0};
  trian = std::make_unique<Triangle>(fbot_right, ftop_right, btop_right);
  trian->SetColor(right_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  trian = std::make_unique<Triangle>(fbot_right, btop_right, bbot_right);
  trian->SetColor(right_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  // Bottom
  Color bot_wall_color{100, 100, 100};
  trian = std::make_unique<Triangle>(fbot_left, fbot_right, bbot_left);
  trian->SetColor(bot_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  trian = std::make_unique<Triangle>(fbot_right, bbot_right, bbot_left);
  trian->SetColor(bot_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  // TOP
  Color top_wall_color{255, 255, 255};
  trian = std::make_unique<Triangle>(ftop_right, ftop_left, btop_left);
  trian->SetColor(top_wall_color)
      .SetMaterial(Material::kLightSource)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  trian = std::make_unique<Triangle>(ftop_right, btop_left, btop_right);
  trian->SetColor(top_wall_color)
      .SetMaterial(Material::kLightSource)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  // BACK
  Color back_wall_color{100, 100, 100};
  trian = std::make_unique<Triangle>(bbot_left, btop_right, btop_left);
  trian->SetColor(back_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));
  trian = std::make_unique<Triangle>(bbot_left, bbot_right, btop_right);
  trian->SetColor(back_wall_color)
      .SetMaterial(wall_material)
      .SetHitPrecision(HIT_PRECISION)
      .SetReflector(std::make_unique<DiffuseReflector>());
  universe.AddObject(std::move(trian));

  return universe;
}

int main() {
  double width = 600;
  double height = 400;
  double scene_depth = 400;
  Scene universe = MakeSimpleRoomScene(width, height, scene_depth, 50.0);
  Camera cam;
  Camera::SetSamplePerPixel(400);
  Pixel::SetBounceLimit(1000);

  size_t px_num = 600 * 400;
  std::vector<Color> col_vec;
  col_vec.reserve(px_num);
  for (size_t i = 0; i < px_num; i++) {
    col_vec.push_back(cam.CreatePixel(i).TracePixel(universe).GetPixelColor());
    if ((10 * i) % px_num == 0) std::cerr << 100.0 * i / px_num << "%\n";
  }
  std::cerr << "Saving file\n";
  std::ofstream image("test.ppm");
  image << "P3\n";
  image << cam.GetWidthInPx() << ' ' << cam.GetHeightInPx() << '\n';
  image << 255 << '\n';
  for (const auto& col : col_vec) {
    image << col << '\n';
  }

  return 0;
}

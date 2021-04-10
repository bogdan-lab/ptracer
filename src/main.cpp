#include "GeoVec.h"
#include "Ray.h"
#include "Objects.h"
#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "Pixel.h"


#include <vector>
#include <fstream>
#include <memory>
#include <utility>

int main(){


    Scene universe;
    ObjHolder source = std::make_unique<Sphere>(GeoVec{100,100,100}, 100.0);
    source->SetColor({255,255,255}).SetMaterial(Material::kLightSource);
    universe.AddObject(std::move(source));

    ObjHolder sphere  = std::make_unique<Sphere>(GeoVec{300, 100, 100}, 100.0);
    sphere->SetColor({255, 129, 63}).SetMaterial(Material::kCommon);
    universe.AddObject(std::move(sphere));

    Camera cam{600, 400, 500};
    std::vector<Pixel> all_pixels = cam.MakeAllPixels();

    for(size_t i=0; i<all_pixels.size(); i++) {
        all_pixels[i].TracePixel(universe, i);
    }

    std::ofstream image("test.ppm");
    image << "P3\n";
    image << cam.GetWidth() << ' ' << cam.GetHeight() << '\n';
    image << 255 << '\n';
    for(const auto& px : all_pixels) {
        image << px.GetPixelColor() << '\n';
    }

    return 0;
}

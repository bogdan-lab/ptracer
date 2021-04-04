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

    Camera cam(42u);
    std::vector<Ray> cam_rays = cam.GetCameraRayGrid();

    Scene universe;
    ObjHolder source = std::make_unique<Sphere>(GeoVec{100,100,100}, 100.0);
    source->SetMaterial(Material::kLightSource);
    universe.AddObject(std::move(source));

    ObjHolder sphere  = std::make_unique<Sphere>(GeoVec{300, 100, 100}, 100.0);
    sphere->SetColor({255, 129, 63}).SetMaterial(Material::kCommon);
    universe.AddObject(std::move(sphere));

    const auto& all_objects = universe.GetObjects();



    /*
    std::ofstream image("test.ppm");
    image << "P3\n";
    image << cam.GetWidth() << ' ' << cam.GetHeight() << '\n';
    image << Color::MAX_COLOR_VALUE << '\n';
    for (size_t i=0; i<cam_rays.size(); i++) {
        if(sphere.GetClosestDist(cam_rays[i])) {
            image << sphere.GetColor() << '\n';
        }
        else {
            image << Color{} << '\n';
        }
    }
    */
    return 0;
}

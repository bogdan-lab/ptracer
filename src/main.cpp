#include "GeoVec.h"
#include "Ray.h"
#include "Objects.h"
#include "Camera.h"

#include <vector>
#include <fstream>

int main(){

    Camera cam;
    std::vector<Ray> cam_rays = cam.GetCameraRayGrid();

    Sphere sphere{{100, 100, 100}, 100};
    sphere.SetColor({255, 139, 87});
    std::ofstream image("test.ppm");
    image << "P3\n";
    image << cam.GetWidth() << ' ' << cam.GetHeight() << '\n';
    image << Color::MAX_COLOR_VALUE << '\n';
    for (size_t i=0; i<cam_rays.size(); i++) {
        if(sphere.GetClosesDist(cam_rays[i])) {
            image << sphere.GetColor() << '\n';
        }
        else {
            image << Color{} << '\n';
        }
    }

    return 0;
}

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

    std::ofstream image("test.pbm");
    image << "P1\n";
    image << cam.GetWidth() << ' ' << cam.GetHeight() << '\n';
    size_t pic_width = cam.GetWidth();
    for (size_t i=0; i<cam_rays.size(); i++) {
        if(sphere.GetClosesDist(cam_rays[i])) {
            image << "1 ";
        }
        else {
            image << "0 ";
        }
        if((i+1)/pic_width>0 && (i+1)%pic_width==0) {
            image << '\n';
        }
    }


    return 0;
}

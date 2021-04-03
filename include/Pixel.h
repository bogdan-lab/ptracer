#ifndef PIXEL_H
#define PIXEL_H

#include "Ray.h"
#include "Color.h"
#include "Scene.h"
#include "Renderer.h"

#include <cstdlib>
#include <vector>


class Pixel {
private:
    static size_t SAMPLE_NUM = 1;
    Ray ray_;
    std::vector<Color> accumulated_colors_;
public:
    Pixel() = delete;

    static void SetSampleNum(size_t g_num) {SAMPLE_NUM = g_num;}

    void TracePixel(const Scene& universe, const Renderer& render) {
        accumulated_colors_.reserve(SAMPLE_NUM);
        for(uint8_t i=0; i<SAMPLE_NUM; i++) {
                accumulated_colors_.push_back(render.RenderRay(ray_, universe));
        }
    }

}




#endif //PIXEL_H
